#pragma once
#include <memory>
#include <iostream>
#include "SeriesBuffer.h"

namespace Nitrade {
	class Indicators
	{

	public:
		Indicators();
		virtual ~Indicators();

		//simple move average
		static double SMA(int periods, Utils::ISeriesBuffer<double>* data);

		//returns true if two dataset have just crossed over
		static bool CrossOver(Utils::ISeriesBuffer<double>* data1, 
			Utils::ISeriesBuffer<double>* data2);

		
	};
}

