#include "OptimiseParameter.h"

std::unique_ptr<double[]> Nitrade::OptimiseParameter::getValues()
{
	int count = ((_max - _min) / _step) + 1;

	auto values = std::make_unique<double[]>(count);

	for (int i = 0; i < count; i++)
		values[i] = i * _step + _min;

	return values;
}
