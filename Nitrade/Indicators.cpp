#include "Indicators.h"
#include <tuple>


Nitrade::Indicators::Indicators()
{
}


Nitrade::Indicators::~Indicators()
{
}

double Nitrade::Indicators::StdDev(int periods, Utils::ISeriesBuffer<double>* data)
{
	//calaculate the mean
	double tally = 0;

	for (int i = 0; i < periods; i++)
		tally += data->get(i);

	double mean = tally / periods;
	
	//calc the stdev
	return StdDev(periods, data, mean);
}

double Nitrade::Indicators::StdDev(int periods, Utils::ISeriesBuffer<double>* data, double mean)
{

	// Compute sum squared  
	// differences with mean. 
	double sqDiff = 0;
	for (int i = 0; i < periods; i++)
		sqDiff += (data->get(i) - mean) * (data->get(i) - mean);
	double variance = sqDiff / periods;

	//return the square root of variance
	return std::sqrt(variance);
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

double Nitrade::Indicators::ATR(int periods, Utils::ISeriesBuffer<double>* closeData, Utils::ISeriesBuffer<double>* highData, Utils::ISeriesBuffer<double>* lowData)
{
	double tally = 0;

	for (int i = 0; i < periods; i++)
	{
		double a = highData->get(i) - lowData->get(i);		
		double b = std::abs(highData->get(i) - closeData->get(i+1));
		double c = std::abs(lowData->get(i) - closeData->get(i + 1));

		double max = std::fmax(a, std::fmax(b, c));

		tally += (double)max;
	}

	return tally / periods;
}


double Nitrade::Indicators::RelativeRangeStdDev(int periodsShort, int periodsLong, Utils::ISeriesBuffer<double>* rangeData)
{
	double shortRange = StdDev(periodsShort, rangeData);
	double longRange = StdDev(periodsLong, rangeData);

	if (longRange != 0)
		return shortRange / longRange;
	else
		return 0.0;
}

double Nitrade::Indicators::LogRelativeRangeStdDev(int periodsShort, int periodsLong, Utils::ISeriesBuffer<double>* rangeData)
{
	double relRange = RelativeRangeStdDev(periodsShort, periodsLong, rangeData);

	//can't take log of zero or less so if this happens put in a realistic value instead of null 
	if (relRange > 0)
		return std::log(relRange);
	else
		return std::log(0.000001);
}

double Nitrade::Indicators::LogRelativeVolume(int periods, Utils::ISeriesBuffer<double>* volume)
{
	double volumeNow = volume->get(0);
	if (volumeNow <= 0)
		volumeNow = 0.000001;
	double volumeSMA = SMA(periods, volume);
	if (volumeSMA <= 0)
		volumeSMA = 0.000001;

	return std::log(volumeNow) / std::log(volumeSMA);
}

std::tuple<double, double, double> Nitrade::Indicators::BollingerBands(int periods, double stddevFactor, Utils::ISeriesBuffer<double>* data)
{
	double sma = SMA(periods, data);
	double stddev = StdDev(periods, data, sma);

	double lower = sma - (stddev * stddevFactor);
	double upper = sma + (stddev * stddevFactor);

	return std::make_tuple(lower, sma, upper);
}

