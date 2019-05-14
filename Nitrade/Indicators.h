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

		static double SMA(int periods, Utils::ISeriesBuffer<double>* data);
		static bool CrossOver(Utils::ISeriesBuffer<double>* data1, Utils::ISeriesBuffer<double>* data2);

		
	};
}

