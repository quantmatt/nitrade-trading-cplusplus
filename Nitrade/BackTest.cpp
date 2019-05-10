#include <iostream>

#include <vector>
#include <time.h>
#include <iterator>

#include "BackTest.h"
#include "IController.h"
#include "PriceData.h"
#include <vector>

Nitrade::BackTest::BackTest()
{
}


Nitrade::BackTest::~BackTest()
{
	
}

void Nitrade::BackTest::RunAll(IController* controller)
{
	std::vector<std::string>* assetNames = controller->getAssetNames();
	if (assetNames != nullptr)
	{
		for (auto name : *assetNames)
			Run(controller, name);
	}

	delete assetNames;
}

void Nitrade::BackTest::Run(IController* controller, std::string assetName)
{
	//check we have a binaryReader attached to the controller
	if (controller == nullptr || !controller->hasBinaryReader(assetName))
		throw std::invalid_argument("A valid IController must be passed that has a binaryChunkReader attached.");

	//open the binary file containing the asset price data for chunk reading
	if(!controller->openFile(assetName))
		throw std::invalid_argument("Controllers binaryChunkReader failed to open file.");

	//get a vector of PriceData objects (one for each timeframe)
	IAsset* asset = controller->getAsset(assetName);

	if (asset == nullptr)
		throw std::invalid_argument(assetName + " has not been setup in memory.");

	//this is a native array of size 50 with a nullptr after the last real item to signify end of array
	//this is 10 x faster than using a vector in the backtest loop.
	Nitrade::IPriceData** priceData = asset->getAllPriceData();

	//get a vector of strategies to run for this asset

	//continue through the file reading it in chunks of size roundedBufferSize
	while(!controller->eof(assetName))
	{
		//get a pointer to the start of the current chunk of binary data
		char* pBinData = controller->getChunk(assetName);
		//also need to get a pointer to the end of the current chunk so we know when to stop iterating
		char* end = controller->endChunk(assetName);

		//throw an error if the bar data appears corrupt due to incorrect binary file
		if(!isBarValid((Bar*)pBinData))
			throw std::invalid_argument("Price data file appears corrupt.");
		
		

		for (char* c = pBinData; c != end; c += sizeof(Bar))
		{
			//casting the pointer to a pointer of Bar is about 10% faster than casting to the struct of Bar
			Bar* bar = (Bar*)c;

			//for (auto it = priceData->begin(); it != priceData->end(); it++) //this loop makes things 10 x slower
			int index = 0;
			while(priceData[index] != nullptr)
			{
				bool isNewBar = priceData[index++]->updateCurrentBarFromBar(bar);

				if (isNewBar)
				{
					//run the strategy code for all loaded strategies that require this timeframe
					controller->onBar(assetName);
				}
			}
					
		}

	}
		

	//release the binary chunk reader
	controller->closeFile(assetName);
	
}

bool Nitrade::BackTest::isBarValid(const Nitrade::Bar* bar)
{
	if (bar == nullptr)
		return false;

	//bar timestamp is less than 1970 or more than the year 3000
	long long val = 1000000000ll;
	long long seconds = bar->timestamp / val;
	if (seconds <= 0 || seconds >  32503683600 ||
		bar->bidHigh < bar->bidLow || bar->askHigh < bar->askLow ||
		bar->volume < 0 || bar->bidOpen < 0 || bar->bidClose < 0 ||
		bar->bidHigh < 0 || bar->bidLow < 0 || bar->askOpen < 0 || 
		bar->askClose < 0 || bar->askHigh < 0 || bar->askLow < 0)
	{
		return false;

	}

	return true;
		
}

