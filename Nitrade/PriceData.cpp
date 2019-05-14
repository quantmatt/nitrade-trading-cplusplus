#include "PriceData.h"
#include "SeriesBuffer.h"
#include <string>




Nitrade::PriceData::PriceData(int lookback, int barSize) : _barSize(barSize)
{
	//create a new series buffer
	_pLookbackBars = new Utils::SeriesBuffer<Nitrade::Bar*>(lookback);	
}

Nitrade::PriceData::PriceData(int lookBack, int barSize, Utils::ISeriesBuffer<Bar*>* mockSeriesBuffer)
{
	_pLookbackBars = mockSeriesBuffer;
}

Nitrade::PriceData::~PriceData()
{
	delete _pLookbackBars;
}

bool Nitrade::PriceData::updateCurrentBarFromBar(Bar* newInfo)
{
	//returns true if this is the start of a new bar

	//peg the timestamp to the nearest bar by converting bar size to nanoseconds and calculating the modulus
	long long peggedNanoseconds = newInfo->timestamp - (newInfo->timestamp % (_barSize * (long long)60000000000));

	//current bar is the bar at index zero - ie. most recent bar
	Bar* pCurrentBar = _pLookbackBars->get(0);

	//new bar if the pegged bar time is different to the current bar time
	if (pCurrentBar == nullptr || peggedNanoseconds != pCurrentBar->timestamp)
	{

		//create a new bar for the higher timeframe using the pegged timestamp and the current bars bid/ask
		//this will create a new pointer that is stored in the array						
		Bar* newBar = new Bar{
			peggedNanoseconds,
			newInfo->bidOpen,
			newInfo->bidClose,
			newInfo->bidHigh,
			newInfo->bidLow,
			newInfo->askOpen,
			newInfo->askClose,
			newInfo->askHigh,
			newInfo->askLow,
			newInfo->volume
		};

		//add this bar to the series
		_pLookbackBars->add(newBar);

		_barIndex++;
		

		//returns true if this is the start of a new bar
		return true;
	}
	//update the current bar
	else
	{
		if (newInfo->bidHigh > pCurrentBar->bidHigh)
			pCurrentBar->bidHigh = newInfo->bidHigh;
		else if (newInfo->bidLow < pCurrentBar->bidLow)
			pCurrentBar->bidLow = newInfo->bidLow;

		if (newInfo->askHigh > pCurrentBar->askHigh)
			pCurrentBar->askHigh = newInfo->askHigh;
		else if (newInfo->askLow < pCurrentBar->askLow)
			pCurrentBar->askLow = newInfo->askLow;

		pCurrentBar->bidClose = newInfo->bidClose;
		pCurrentBar->askClose = newInfo->askClose;

		return false;
	}

}

bool Nitrade::PriceData::updateCurrentBarFromTick(float quote, bool isBid)
{
	//Not yet implemented
	return false;
}

std::string Nitrade::PriceData::getName()
{
	std::string description = std::to_string(_barSize) + "min";
	return description;
}

int Nitrade::PriceData::getBarIndex()
{
	return _barIndex;
}
