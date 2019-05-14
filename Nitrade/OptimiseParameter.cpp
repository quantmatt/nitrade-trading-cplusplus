#include "OptimiseParameter.h"

std::vector<double> Nitrade::OptimiseParameter::getValues()
{
	int count = ((_max - _min) / _step) + 1;

	auto values = std::vector<double>(count);

	for (int i = 0; i < count; i++)
		values[i] = i * _step + _min;

	return values;
}

std::string Nitrade::OptimiseParameter::getName()
{
	return _name;
}
