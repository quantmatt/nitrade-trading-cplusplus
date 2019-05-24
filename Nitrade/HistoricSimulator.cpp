#include "HistoricSimulator.h"
#include "DataFactory.h"
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
void Nitrade::HistoricSimulator::Setup(std::unique_ptr<IStrategyDefinition> strategyDef, 
	std::unique_ptr<IDataFactory> dataFactory)
{
	//setup the strategy definition and datafactory
	_strategyDefinition = std::move(strategyDef);
	if (dataFactory != nullptr)
		_dataFactory = std::move(dataFactory);
	else
		_dataFactory = std::make_unique<Nitrade::DataFactory>();

	


}

void Nitrade::HistoricSimulator::optimise(int cpus, std::vector<std::string> assets, bool runningPL)
{
	//clear the files if they exist
	remove("trades.bin");
	remove("trades_data.bin");
	remove("daily_returns.bin");

	//calculte how many assets will run on each thread - round up to nearest int
	int assetsPerThread = (int)(std::ceil((double)assets.size() / (double)cpus));

	//create a pool of threads to used - assets are split up amoungst the threads
	auto threads = std::vector<std::unique_ptr<std::thread>>();

	unsigned int assetIndex = 0;
	for (int i=0; i < cpus; i++)
	{

		//get a list of assets for this thread
		std::vector<std::string> assetsForThread;
		for (int z = 0; z < assetsPerThread; z++)
		{			
			if (assetIndex < assets.size())	
				assetsForThread.push_back(assets[assetIndex]);

			assetIndex++;
		}

		//create a tradeManager for each thread here so that we can write all trades from the trademanager in in a single thread
		//at the end of the process to avoid multiple file accesses from multiple threads
		_threadTradeManagers.push_back(std::move(_dataFactory->getTradeManager()));
		//get a pointer to this tradeManager for the thread to use
		auto ptr = _threadTradeManagers[_threadTradeManagers.size() - 1].get();

		//start the thread
		threads.push_back(std::make_unique<std::thread>(&HistoricSimulator::optimiseAssets, this, ptr, assetsForThread, runningPL));
		
		//no more threads needed
		if (assetIndex >= assets.size())
			break;
	
	}

	//wait until threads finish
	for (auto& thread : threads)	
		(*thread).join();
	

	//write all trades from all trade managers to the trade files using appending to file
	bool includeHeader = true;
	int startTradeId = 0;
	for (auto& tm : _threadTradeManagers)
	{		
		//write all trades to a binary file
		//the startTradeId allows the other trademanagers to update the tradeId's to carry on from the number of the last trademanager
		tm->writeTradeDataToBinary("trades_data.bin", startTradeId);
		startTradeId = tm->writeTradesToBinary("trades.bin", startTradeId);		
		includeHeader = false;
		if (runningPL)
			tm->writeRunningPLToBinary("daily_returns.bin");
		
	}

}



void Nitrade::HistoricSimulator::optimise(std::string assetName, bool runningPL)
{
	//write all trades to a binary file
	remove("trades.bin");
	remove("trades_data.bin");
	remove("daily_returns.bin");

	auto tradeManager = _dataFactory->getTradeManager();
	std::vector<std::string> assets = { assetName };

	optimiseAssets(tradeManager.get(), assets, runningPL);

	//write all trades to a binary file
	tradeManager->writeTradesToBinary("trades.bin");
	tradeManager->writeTradeDataToBinary("trades_data.bin");
	if (runningPL)
		tradeManager->writeRunningPLToBinary("daily_returns.bin");

}

void Nitrade::HistoricSimulator::optimiseAssets(ITradeManager* tradeManager, std::vector<std::string> assets, bool runningPL)
{
	
	//get all the objects required to run the test using the data factory class
	
	//load all the asset details into the trade manager
	tradeManager->loadAssetDetails();

	//run all the assets required for this thread
	for (auto assetName : assets)
	{

		//The details about the asset eg. datapath for binary, digits, pip value ect.
		auto asset = tradeManager->getAsset(assetName);

		//A set of strategies generated with all possible values of the input variables
		//for this particular asset
		auto strategies = _dataFactory->getStrategySet(_strategyDefinition.get(), asset);

		//create the price data arrays for this asset.
		//these will be filled as we traverse through the binary data
		auto assetData = _dataFactory->getAssetData(_strategyDefinition.get());
		int size = assetData->size();

		//Create a binary chunk reader to read the binary price data in chunks
		//chunks not only conserve memory use but also process faster
		auto bcr = _dataFactory->getBinaryChunkReader(asset->getDataPath());


		//initialise all the strategies
		strategies->init(tradeManager, assetData.get());

		//get a list of the keys used to reference the strategies
		auto keys = strategies->getStrategyKeys();


		//All objects have been created so start processing the binary data into OHLC bars
		bcr->openFile();

		//declare a variable to hold the last known daily bar timestamp
		//to determine if a new daily bar has formed - for calculationg daily PL
		long long currentDailyBar = 0;

		//continue through the file reading it in chunks of size roundedBufferSize
		while (!bcr->eof())
		{
			//get a pointer to the start of the current chunk of binary data
			char* pBinData = bcr->getChunk();
			//also need to get a pointer to the end of the current chunk so we know when to stop iterating
			char* end = bcr->endChunk();

			//throw an error if the bar data appears corrupt due to incorrect binary file
			if (!isBarValid((Bar*)pBinData))
				throw std::invalid_argument("Price data file appears corrupt.");


			for (char* c = pBinData; c != end; c += sizeof(Bar))
			{
				//casting the pointer to a pointer of Bar is about 10% faster than casting to the struct of Bar
				Bar* bar = (Bar*)c;

				for (int i = 0; i < size; i++)
				{
					bool isNewBar = (*assetData)[i]->updateCurrentBarFromBar(bar);

					if (isNewBar)
					{
						//run the strategy code for all loaded strategies that require this timeframe					
						strategies->run((*assetData)[i]);
					}

				}

				//update the open trades after all the onBar methods run - for example if current minute bar is
				//7:00 open to 7:01 close then the 7:00 onBar function should run first and then essentially this
				//updateOpenTrades function is run theoretically at 7.01 ie. 1 min has passed between onBar and now
				tradeManager->updateOpenTrades(bar);

				//if this is a new bar based on a daily bar then update the running daily Profit Loss
				//for each strategy
				if (runningPL && bar->timestamp > currentDailyBar + 86400000000000)
				{
					currentDailyBar = bar->timestamp - (bar->timestamp % 86400000000000);
					tradeManager->onDay(keys, currentDailyBar); //run updates for daily stats eg runnning PL

				}

			}

		}


		bcr->closeFile();
		//finished so close the binary file
	}

	
}

bool Nitrade::HistoricSimulator::isBarValid(const Nitrade::Bar* bar)
{
	if (bar == nullptr)
		return false;

	//bar timestamp is less than 1970 or more than the year 3000
	long long val = 1000000000ll;
	long long seconds = bar->timestamp / val;
	if (seconds <= 0 || seconds > 32503683600 ||
		bar->bidHigh < bar->bidLow || bar->askHigh < bar->askLow ||
		bar->volume < 0 || bar->bidOpen < 0 || bar->bidClose < 0 ||
		bar->bidHigh < 0 || bar->bidLow < 0 || bar->askOpen < 0 ||
		bar->askClose < 0 || bar->askHigh < 0 || bar->askLow < 0)
	{
		return false;

	}

	return true;

}


