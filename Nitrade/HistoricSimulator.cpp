#include "HistoricSimulator.h"
#include "DataFactory.h"

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

void Nitrade::HistoricSimulator::Optimise(int threads)
{

}

void Nitrade::HistoricSimulator::Optimise(std::string assetName, bool runningPL)
{
	//get all the objects required to run the test using the data factory class
	auto tradeManager = _dataFactory->getTradeManager();
	//load all the asset details into the trade manager
	tradeManager->loadAssetDetails();

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
	strategies->init(tradeManager.get(), assetData.get());

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

	//write all trades to a binary file
	tradeManager->writeTradesToBinary("trades.bin");
	tradeManager->writeTradeDataToBinary("trades_data.bin");
	if(runningPL)
		tradeManager->writeRunningPLToBinary("daily_returns.bin");

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


