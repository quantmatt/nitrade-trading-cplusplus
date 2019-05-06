#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <iterator>

#include "BackTest.h"


Nitrade::BackTest::BackTest()
{
	std::cout << "BackTest init called" << std::endl;	
}


Nitrade::BackTest::~BackTest()
{
}

double Nitrade::BackTest::ChunkArray(int bufferSizeMB)
{
	//convert megabytes to bytes
	int bufferSize = bufferSizeMB * 1024 * 1024;

	//round the buffer size to whole bars
	int roundedBufferSize = (bufferSize / sizeof(Bar)) * sizeof(Bar); 

	clock_t t;
	t = clock();

	int longTrades = 0;
	int shortTrades = 0;

	std::ifstream inputFile;

	//reading from D drive seems to be about 10% faster again - maybe because program is running on D drive
	//std::string filename = "C:\\ForexData\\BaseData\\EURUSD_m1.bin";
	std::string filename = "D:\\TickData\\EURUSD_m1.bin";

	inputFile.open(filename, std::ios::binary | std::ios::ate);

	//declare a pointer to hold the in-memory binary data
	char* binData = NULL;

	if (inputFile.is_open())
	{
		//get the size of the file and move to start of file
		const std::streamsize size = inputFile.tellg();
		inputFile.seekg(0, std::ios::beg);

		//don't allow the buffer to be larger than the actual file size
		if (roundedBufferSize > size)
			roundedBufferSize = size;

		//declare the binaryData array to hold the whole contents of the file.
		binData = new char[roundedBufferSize];

		int lookback = 200;
		int arrayFactor = 100;
		int processedBytes = 0;
		long long barSize = 60;
		long long currentBarTime = 0;

		//create a pointer to an array of lookback bar data for this timeframe - make the array much larger then needed to reduce time in copying array
		int arraySize = lookback * arrayFactor;
		Bar* pLookbackBars = new Bar[arraySize];

		//use an indexer to specify the current start pointer of the lookback period		
		Bar* pCurrentBar =  NULL;
		int lookbackIndex = 0;

		

		//continue through the file reading it in chunks of size roundedBufferSize
		while(!inputFile.eof())
		{
			//make sure we don't read more than what is left
			int remaining = size - processedBytes;
			if (remaining < roundedBufferSize)
				roundedBufferSize = remaining+1;

			//read the file into the char array
			if (inputFile.read(binData, roundedBufferSize))
			{
				processedBytes += roundedBufferSize;

				char* lastByte = binData + roundedBufferSize;

				for (char* c = binData; c != lastByte; c += sizeof(Bar))
				{
					//casting the pointer to a pointer of Bar is about 10% faster than casting to the struct of Bar
					Bar* bar = (Bar*)c;
				
					//peg the timestamp to the nearest bar start
					//int seconds = bar->timestamp / 1000000000;
					//int mod_seconds = (int)(seconds % (barSize * 60));
					//long long peggedNanoseconds = seconds - (long long)mod_seconds;
					//int seconds = bar->timestamp / 1000000000;

					long long peggedNanoseconds = bar->timestamp - (bar->timestamp % (barSize * (long long)60000000000));

					//new bar					
					if (peggedNanoseconds != currentBarTime)
					{
						//update the current bar timestamp
						currentBarTime = peggedNanoseconds;

						//create a new bar for the higher timeframe using the pegged timestamp and the current bars bid/ask
						//this will create a new pointer that is stored in the array						
						Bar* newBar = new Bar{
							peggedNanoseconds,
							bar->bidOpen,
							bar->bidClose,
							bar->bidHigh,
							bar->bidLow,
							bar->askOpen,
							bar->askClose,
							bar->askHigh,
							bar->askLow,
							bar->volume
						};
						pLookbackBars[lookbackIndex] = *newBar;
						pCurrentBar = newBar;

						//////////////////////////////////////////////////
						//Run strategy code here
						//////////////////////////////////////////////////
						for (int i = 0; i < 1; i++)
						{

							//calc sma
							if (lookbackIndex > 50)
							{
							
								float tally = 0;
								for (int z = 0; z < 50; z++)
								{
									tally += pLookbackBars[lookbackIndex - z].askClose;
								}
								float sma = tally / 50;

								float lowCut = sma * (1 - ((float)1 / 50));
								float highCut = sma * (1 + ((float)1 / 50));
								if (pLookbackBars[lookbackIndex].askClose > highCut)
								{
									Trade trade;
									trade.openTime = newBar->timestamp;
									trade.openLevel = newBar->askOpen;
									trade.spread = (int)((newBar->askOpen - newBar->bidOpen) / (float)0.00001);
									trade.isLong = true;									
									trades.push_back(trade);
										
								}
								else if (pLookbackBars[lookbackIndex].askClose < lowCut)
								{
									Trade trade;
									trade.openTime = newBar->timestamp;
									trade.openLevel = newBar->bidOpen;
									trade.spread = (int)((newBar->askOpen - newBar->bidOpen) / (float)0.00001);
									trade.isLong = false;
									trades.push_back(trade);
								}
							}

						}


						//move the pointer to the next bar in the array
						lookbackIndex++;
						
						//if this is the end of our array - copy lookback bars back to the start of the same array
						if (lookbackIndex >= arraySize)
						{
				
							std::copy(&pLookbackBars[arraySize - lookback+1], &pLookbackBars[arraySize], pLookbackBars);

							//set the current bar to the end of the lookback period
							lookbackIndex = lookback-1;

						}

						//convert nanoseconds to seconds
						//time_t timeSinceEpochPegged = peggedNanoseconds / 1000000000;
						//tm timeResultPegged;
						//gmtime_s(&timeResultPegged, &timeSinceEpochPegged);
					}
					//update the current bar
					else if(pCurrentBar != NULL)
					{
						if (bar->bidHigh > pCurrentBar->bidHigh)
							pCurrentBar->bidHigh = bar->bidHigh;
						else if (bar->bidLow < pCurrentBar->bidLow)
							pCurrentBar->bidLow = bar->bidLow;

						if (bar->askHigh > pCurrentBar->askHigh)
							pCurrentBar->askHigh = bar->askHigh;
						else if (bar->askLow < pCurrentBar->askLow)
							pCurrentBar->askLow = bar->askLow;

						pCurrentBar->bidClose = bar->bidClose;
						pCurrentBar->askClose = bar->askClose;
					}
					
					
					
				}
			}


		}

		delete[] pLookbackBars;

	}

	if (binData != NULL)
		delete[] binData;
	


	t = clock() - t;
	std::cout << "time: " << t << " miliseconds" << std::endl;
	std::cout << CLOCKS_PER_SEC << " clocks per second" << std::endl;
	std::cout << "time: " << t * 1.0 / CLOCKS_PER_SEC << " seconds" << std::endl;

	inputFile.close();

	return (t * 1.0 / CLOCKS_PER_SEC);
}

