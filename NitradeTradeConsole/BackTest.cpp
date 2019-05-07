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
	
	//open the binary file containing the asset price data for chunk reading
	controller->openFile();

	//get a vector of PriceData objects (one for each timeframe)
	std::vector<Nitrade::PriceData>* priceData = controller->getAssetData(assetName);	

	//get a vector of strategies to run for this asset

	//continue through the file reading it in chunks of size roundedBufferSize
	while(!controller->eof())
	{
		//get a pointer to the start of the current chunk of binary data
		char* pBinData = controller->getChunk();
		//also need to get a pointer to the end of the current chunk so we know when to stop iterating
		char* end = controller->endChunk();

		for (char* c = pBinData; c != end; c += sizeof(Bar))
		{
			//casting the pointer to a pointer of Bar is about 10% faster than casting to the struct of Bar
			Bar* bar = (Bar*)c;

			//foreach pricedata object check if new bar or update current bar
			for (std::vector<Nitrade::PriceData>::iterator it = priceData->begin(); it != priceData->end(); it++)
			{		
				bool isNewBar = it->updateCurrentBarFromBar(bar);

				if (isNewBar)
				{
					//run the strategy code for all loaded strategies that require this timeframe
				}
			}	
					
		}
		
		if (pBinData != NULL)
			delete[] pBinData;
	}
		

	//release the binary chunk reader
	controller->closeFile();
	
}

