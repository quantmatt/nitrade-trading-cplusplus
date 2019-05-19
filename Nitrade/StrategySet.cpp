#include "StrategySet.h"
#include <memory>
#include <math.h>


Nitrade::StrategySet::StrategySet()
{
}


Nitrade::StrategySet::~StrategySet()
{
}

void Nitrade::StrategySet::createFrom(IStrategyDefinition* strategyDefintion,  Nitrade::Asset* asset)
{
	//Does a brute force calculation of all the possible strategy setups with the range of optimisable
	//parameters. A parameter has a start value, step value and max value

	//get the number of optimisable parameters for the given strategy
	int paramCount = strategyDefintion->getOptimiseParameterCount();

	//get the base strategy to use as a template
	Strategy* baseStrategy = strategyDefintion->getStrategy();
	baseStrategy->setAsset(asset);

	//if no optimisiation just set the strategy set as a single variant.
	if (paramCount == 0)
	{
		_strategies = std::vector<std::unique_ptr<Strategy>>(1);
		_strategies[0] = std::move(baseStrategy->clone());
		_strategyCount = 1;
		return;			
	}

	//create a vector to hold all the possible values for each optimisiable parameter
	auto paramValues = std::vector<std::vector<double>>(paramCount);

	//calculate how many strategies will be created from a brute force optimisation of the parameters	
	//during the loop below by multiplying the number by the number of values for each parameter
	int count = 1;

	//create an array of values for each optimise parameter
	for (int i = 0; i < paramCount; i++)
	{
		OptimiseParameter op = strategyDefintion->getOptimiseParams(i);
		
		paramValues[i] = op.getValues();
		count = count * paramValues[i].size();
	}

	
	_strategyCount = count;

	//create an array large enough to hold all the strategies
	_strategies = std::vector<std::unique_ptr<Strategy>>(count);


	//algorithm below creates the cartesian product of all the possible parameter values
	int sIndex = 0;
	auto indexArray = std::vector<unsigned int>(paramCount);

	int floater = paramCount - 1;

	bool exitLoop = false;
	while (indexArray[floater] < paramValues[floater].size() && !exitLoop)
	{
		_strategies[sIndex] = std::move(baseStrategy->clone());

		//give each strategy a unique index so we can track trades
		_strategies[sIndex]->setVariantId(sIndex); 

		//assing the same asset
		_strategies[sIndex]->setAsset(asset);

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
	//sets each strategy in the set with the required assetdata and trade manager objects
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
