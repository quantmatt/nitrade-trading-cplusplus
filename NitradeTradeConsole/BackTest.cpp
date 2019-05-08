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

void Nitrade::BackTest::Run(IController* controller, std::string assetName)
{
	//check we have a binaryReader attached to the controller
	if (controller == nullptr || !controller->hasBinaryReader())
		throw std::invalid_argument("A valid IController must be passed that has a binaryChunkReader attached.");

	//open the binary file containing the asset price data for chunk reading
	if(!controller->openFile())
		throw std::invalid_argument("Controllers binaryChunkReader failed to open file.");

	//get a vector of PriceData objects (one for each timeframe)
	std::vector<Nitrade::IPriceData*>* priceData = controller->getAssetData(assetName);	

	//get a vector of strategies to run for this asset

	//continue through the file reading it in chunks of size roundedBufferSize
	while(!controller->eof())
	{
		//get a pointer to the start of the current chunk of binary data
		char* pBinData = controller->getChunk();
		//also need to get a pointer to the end of the current chunk so we know when to stop iterating
		char* end = controller->endChunk();

		//throw an error if the bar data appears corrupt due to incorrect binary file
		if(!isBarValid((Bar*)pBinData))
			throw std::invalid_argument("Price data file appears corrupt.");
		

		for (char* c = pBinData; c != end; c += sizeof(Bar))
		{
			//casting the pointer to a pointer of Bar is about 10% faster than casting to the struct of Bar
			Bar* bar = (Bar*)c;

			//foreach pricedata object check if new bar or update current bar
			for (std::vector<Nitrade::IPriceData*>::iterator it = priceData->begin(); it != priceData->end(); it++)
			{		
				bool isNewBar = (*it)->updateCurrentBarFromBar(bar);

				if (isNewBar)
				{
					//run the strategy code for all loaded strategies that require this timeframe
					controller->onBar();					
				}
			}	
					
		}

	}
		

	//release the binary chunk reader
	controller->closeFile();
	
}

bool Nitrade::BackTest::isBarValid(const Nitrade::Bar* bar)
{
	if (bar == nullptr)
		return false;

	//bar timestamp is less than 1970 or more than the year 3000
	long long val = 1000000000ll;
	long long seconds = bar->timestamp / val;
	if (seconds <= 0 || seconds >  32503683600 ||
		bar->bidHigh > bar->bidLow || bar->askHigh > bar->askLow ||
		bar->volume < 0 || bar->bidOpen < 0 || bar->bidClose < 0 ||
		bar->bidHigh < 0 || bar->bidLow < 0 || bar->askOpen < 0 || 
		bar->askClose < 0 || bar->askHigh < 0 || bar->askLow < 0)
	{
		return false;

	}

	return true;
		
}

