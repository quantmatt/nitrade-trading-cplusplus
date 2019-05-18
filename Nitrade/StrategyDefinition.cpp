#include "StrategyDefinition.h"
#include "OptimiseParameter.h"

Nitrade::StrategyDefinition::StrategyDefinition()
{
	//hard code these for now
	//later will construct this info frm the statregy script

	_dataSetCount = 1;
	_dataSetParameters = std::make_unique<std::tuple<std::string, int, int>[]>(_dataSetCount);
	_dataSetParameters[0] = std::make_tuple("60min", 200, 60);
	

	_optimiseParameterCount = 3;
	_optimiseParameters = std::make_unique<OptimiseParameter[]>(_optimiseParameterCount);
	_optimiseParameters[0] = OptimiseParameter("Period1", 4, 5, 1, 6);
	_optimiseParameters[1] = OptimiseParameter("Period2", 14, 15, 1, 16);
	_optimiseParameters[2] = OptimiseParameter("Not Used", 1, 1, 1, 1);




}

void Nitrade::StrategyDefinition::setStrategy(std::unique_ptr<Strategy> strategy)
{
	_strategy = std::move(strategy);
}

Nitrade::Strategy* Nitrade::StrategyDefinition::getStrategy()
{
	return _strategy.get();
}

const std::tuple<std::string, int, int>& Nitrade::StrategyDefinition::getDataSetParams(int index)
{
	return std::move(_dataSetParameters[index]);	
}

int Nitrade::StrategyDefinition::getDataSetCount()
{
	return _dataSetCount;
}

const Nitrade::OptimiseParameter& Nitrade::StrategyDefinition::getOptimiseParams(int index)
{
	return _optimiseParameters[index];
}

int Nitrade::StrategyDefinition::getOptimiseParameterCount()
{
	return _optimiseParameterCount;
}
