#include "StrategySet.h"
#include <memory>



Nitrade::StrategySet::StrategySet()
{
}


Nitrade::StrategySet::~StrategySet()
{
}

void Nitrade::StrategySet::createFrom(IStrategyDefinition* strategyDefintion)
{
	auto paramValues = std::make_unique<std::unique_ptr<double[]>[]>(strategyDefintion->getOptimiseParameterCount());

	//create an array of values for each optimise parameter
	for (int i = 0; i < strategyDefintion->getOptimiseParameterCount(); i++)
	{
		OptimiseParameter op = strategyDefintion->getOptimiseParams(i);
		paramValues[i] = std::move(op.getValues());

	}
}

void Nitrade::StrategySet::run(IPriceData* dataSet)
{
	//loop through all the strategies in the set
	for (int i = 0; i < _strategyCount; i++)
	{
		//set the current dataset in the strategy, returns true if this strategy will run this dataset
		//run the onbar function if this strategy should run the dataset
		if (_strategies[i].setCurrentDataIfRequired(dataSet))
			_strategies[i].onBar();
	}
}
