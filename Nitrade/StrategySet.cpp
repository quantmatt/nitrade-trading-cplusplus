#include "StrategySet.h"
#include <memory>
#include <math.h>


Nitrade::StrategySet::StrategySet()
{
}


Nitrade::StrategySet::~StrategySet()
{
}

void Nitrade::StrategySet::createFrom(IStrategyDefinition* strategyDefintion)
{
	int paramCount = strategyDefintion->getOptimiseParameterCount();

	//get the base strategy to use as a template
	Strategy* baseStrategy = strategyDefintion->getStrategy();

	//if no optimisiation just set the strategy as is.
	if (paramCount == 0)
	{
		_strategies = std::vector<std::unique_ptr<Strategy>>(1);
		_strategies[0] = std::move(baseStrategy->clone());
		_strategyCount = 1;
		return;			
	}

	auto paramValues = std::vector<std::vector<double>>(paramCount);

	int count = 1;

	//create an array of values for each optimise parameter
	for (int i = 0; i < paramCount; i++)
	{
		OptimiseParameter op = strategyDefintion->getOptimiseParams(i);
		
		paramValues[i] = op.getValues();
		count = count * paramValues[i].size();
	}

	//calculate how many strategies will be created from a brute force optimisation of the parameters
	
	_strategyCount = count;
	//create an array large enough to hold all the strategies
	_strategies = std::vector<std::unique_ptr<Strategy>>(count);


	int sIndex = 0;
	auto indexArray = std::vector<unsigned int>(paramCount);

	int floater = paramCount - 1;

	bool exitLoop = false;
	while (indexArray[floater] < paramValues[floater].size() && !exitLoop)
	{
		_strategies[sIndex] = std::move(baseStrategy->clone());

		for (int i = 0; i < paramCount; i++)
		{
			OptimiseParameter op = strategyDefintion->getOptimiseParams(i);
			std::string paramName = op.getName();
			_strategies[sIndex]->setParameter(paramName, paramValues[i][indexArray[i]]);
		}

		sIndex++;


		if (indexArray[floater] >= paramValues[floater].size() - 1)
		{
			indexArray[floater] = 0;

			int carry = floater - 1;
			while (carry >= 0)
			{
				if (indexArray[carry] >= paramValues[carry].size() - 1)
				{
					indexArray[carry] = 0;
					carry--;
					if (carry < 0)
						exitLoop = true;
				}
				else
				{
					indexArray[carry]++;
					break;
				}

			}
		}
		else
		{
			indexArray[floater]++;
		}

		
	}
	
}

void Nitrade::StrategySet::init(ITradeManager* tradeManager, IAssetData* assetData)
{
	for (int i = 0; i < _strategyCount; i++)
	{
		_strategies[i]->init(tradeManager, assetData);
	}
}

void Nitrade::StrategySet::run(IPriceData* dataSet)
{
	//loop through all the strategies in the set
	for (int i = 0; i < _strategyCount; i++)
	{
		//set the current dataset in the strategy, returns true if this strategy will run this dataset
		//run the onbar function if this strategy should run the dataset
		if (_strategies[i]->setCurrentDataIfRequired(dataSet))
			_strategies[i]->onBar();
	}
}
