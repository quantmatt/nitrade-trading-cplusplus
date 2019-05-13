#include "StrategyDefinition.h"

Nitrade::StrategyDefinition::StrategyDefinition()
{
	int size = 1;
	_dataSetParameters = std::make_unique<std::tuple<std::string, int, int>[]>(size);
	_dataSetParameters[0] = std::make_tuple("60min", 200, 60);
}

const std::tuple<std::string, int, int>& Nitrade::StrategyDefinition::getDataSetParams(int index)
{
	return std::move(_dataSetParameters[index]);	
}

int Nitrade::StrategyDefinition::getDataSetCount()
{
	return _dataSetCount;
}
