#include "PriceData.h"
#include <iterator>




Nitrade::PriceData::PriceData()
{
}

Nitrade::PriceData::~PriceData()
{
	if(_pLookbackBars != nullptr)
		delete[] _pLookbackBars;
}

void Nitrade::PriceData::init(int lookback, int barSize, int arraySizeFactor)
{
	_lookback = lookback;
	_barSize = barSize;

	//create a pointer to an array of lookback bar data for this timeframe
	//make the array is much larger then needed to reduce time in copying array
	_arraySize = lookback * arraySizeFactor;

	//initialise the array to hold the most recent bar price data 
	_pLookbackBars = new Bar[_arraySize];

	//start the lookback index at zerp
	_lookbackIndex = 0;

	//the current bar being built
	_pCurrentBar = nullptr;


}

bool Nitrade::PriceData::updateCurrentBarFromBar(Bar* newInfo)
{
	//returns true if this is the start of a new bar

	//peg the timestamp to the nearest bar by converting bar size to nanoseconds and calculating the modulus
	long long peggedNanoseconds = newInfo->timestamp - (newInfo->timestamp % (_barSize * (long long)60000000000));

	//new bar if the pegged bar time is different to the current bar time
	if (_pCurrentBar == nullptr || peggedNanoseconds != _pCurrentBar->timestamp)
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

		addNewBar(newBar);
		

		//returns true if this is the start of a new bar
		return true;
	}
	//update the current bar
	else
	{
		if (newInfo->bidHigh > _pCurrentBar->bidHigh)
			_pCurrentBar->bidHigh = newInfo->bidHigh;
		else if (newInfo->bidLow < _pCurrentBar->bidLow)
			_pCurrentBar->bidLow = newInfo->bidLow;

		if (newInfo->askHigh > _pCurrentBar->askHigh)
			_pCurrentBar->askHigh = newInfo->askHigh;
		else if (newInfo->askLow < _pCurrentBar->askLow)
			_pCurrentBar->askLow = newInfo->askLow;

		_pCurrentBar->bidClose = newInfo->bidClose;
		_pCurrentBar->askClose = newInfo->askClose;

		return false;
	}

}

bool Nitrade::PriceData::updateCurrentBarFromTick(float quote, bool isBid)
{
	//Not yet implemented
	return false;
}

void Nitrade::PriceData::addNewBar(Bar* newBar)
{
	//move the pointer to the next bar in the array
	_lookbackIndex++;

	//if this is the end of our array - copy lookback bars back to the start of the same array
	if (_lookbackIndex >= _arraySize)
		refreshInternalArray();

	//put the bar in the array at the current index
	_pLookbackBars[_lookbackIndex] = *newBar;

	//keep a pointer to the current bar
	_pCurrentBar = newBar;
}

void Nitrade::PriceData::refreshInternalArray()
{
	//copy lookback bars back to the start of the same array
	std::copy(&_pLookbackBars[_arraySize - _lookback + 1], &_pLookbackBars[_arraySize], _pLookbackBars);

	//set the current bar to the end of the lookback period
	_lookbackIndex = _lookback - 1;
}
