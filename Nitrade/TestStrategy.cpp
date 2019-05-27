#include "TestStrategy.h"

Nitrade::TestStrategy::TestStrategy()
{
	_dataSetCount = 1;
	_dataSetParameters = std::make_unique<std::tuple<std::string, int, int>[]>(_dataSetCount);
	_dataSetParameters[0] = std::make_tuple("240min", 200, 240);


	_optimiseParameterCount = 3;
	_optimiseParameters = std::make_unique<OptimiseParameter[]>(_optimiseParameterCount);
	_optimiseParameters[0] = OptimiseParameter("Period1", 5, 30, 2, 6);
	_optimiseParameters[1] = OptimiseParameter("Period2", 20, 50, 3, 16);
	_optimiseParameters[2] = OptimiseParameter("Not Used", 1, 1, 1, 1);
}
