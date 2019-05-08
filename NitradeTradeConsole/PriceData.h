#pragma once

#include <iostream>
#include "IPriceData.h"
#include "Structs.h"

namespace Nitrade {
	class PriceData : 
		public IPriceData
	{
	private:				
		Nitrade::Bar* _pCurrentBar{}; //The current bar in progress

		int _lookback{0};
		int _barSize{0};
		int _arraySize{0};
		long long _currentBarTime{0};

		//the array (as a series) of recent price data
		Nitrade::Bar* _pLookbackBars{};

		//the lookback bar array is much larger then the required lookback period for speed in backtested
		//the lookback index is the current index of the bar in the larger array ie. |oooounusedoooo|---lookback area --------|oooounsuedooooooo|
		int _lookbackIndex{0};
	public:
		PriceData();
		virtual ~PriceData();

		void init(int lookback, int barSize, int arraySizeFactor = 100);
		bool updateCurrentBarFromBar(Nitrade::Bar* newInfo); //used update current bar from minute bar
		bool updateCurrentBarFromTick(float quote, bool isBid); //used to update current bar on a new tick 

	private:
		void addNewBar(Nitrade::Bar* newBar);
		void refreshInternalArray();
	};
}

