#pragma once
#include <memory>
#include <iostream>
#include <math.h>
#include "SeriesBuffer.h"

namespace Nitrade {
	class Indicators
	{

	public:
		Indicators();
		virtual ~Indicators();

		//StdDev and Mean
		static double StdDev(int periods, Utils::ISeriesBuffer<double>* data);
		static double StdDev(int periods, Utils::ISeriesBuffer<double>* data, double mean); //if mean is already calculated

		//simple move average
		static double SMA(int periods, Utils::ISeriesBuffer<double>* data);

		//returns true if two dataset have just crossed over
		static bool CrossOver(Utils::ISeriesBuffer<double>* data1, 
			Utils::ISeriesBuffer<double>* data2);

		//Average True Range
		static double ATR(int periods,
			Utils::ISeriesBuffer<double>* closeData, Utils::ISeriesBuffer<double>* highData, Utils::ISeriesBuffer<double>* lowData);

		//Relative Range StdDev - just the stddev of the high minus the low divided by a much larger period
		static double RelativeRangeStdDev(int periodsShort, int periodsLong,
			Utils::ISeriesBuffer<double>* rangeData);

		//Log of Relative Range StdDev - just the stddev of the high minus the low divided by a much larger period then take the log value
		static double LogRelativeRangeStdDev(int periodsShort, int periodsLong,
			Utils::ISeriesBuffer<double>* rangeData);

		//Log of Relative Volume - just the log of 
		static double LogRelativeVolume(int periods,
			Utils::ISeriesBuffer<double>* volume);

		//Bollinger Bands returns lower, middle, uper in a tuple
		static std::tuple<double, double, double> BollingerBands(int periods, double stddevFactor,
			Utils::ISeriesBuffer<double>* data);

	};
}

