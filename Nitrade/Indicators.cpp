#include "Indicators.h"



Nitrade::Indicators::Indicators()
{
}


Nitrade::Indicators::~Indicators()
{
}

double Nitrade::Indicators::SMA(int periods, Utils::ISeriesBuffer<double>* data)
{
	//calculate the sma
	double tally = 0;
	for (int i = 0; i < periods; i++)
		tally += data->get(i);

	double sma = tally / periods;

	return sma;
}

bool Nitrade::Indicators::CrossOver(Utils::ISeriesBuffer<double>* data1, Utils::ISeriesBuffer<double>* data2)
{
	if ((data1->get(0) > data2->get(0) && data1->get(1) < data2->get(1)) ||
		(data2->get(0) > data1->get(0) && data2->get(1) < data1->get(1)))
		return true;

	return false;
}

