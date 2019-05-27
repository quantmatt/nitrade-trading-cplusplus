#pragma once
#include "NitradeLib.h"


using namespace Nitrade;
using namespace std;

class TestStrategy :
	public Strategy
{
public:
	TestStrategy();
	~TestStrategy();
};



TestStrategy::TestStrategy()
{
	_dataSetParameters = std::vector<std::tuple<std::string, int, int>>(1);
	_dataSetParameters[0] = std::make_tuple("240min", 200, 240);

	_optimiseParameters = std::vector<OptimiseParameter>(2);
	_optimiseParameters[0] = OptimiseParameter("Period1", 5, 7, 1, 6);
	_optimiseParameters[1] = OptimiseParameter("Period2", 20, 22, 1, 21);
}


TestStrategy::~TestStrategy()
{
}
