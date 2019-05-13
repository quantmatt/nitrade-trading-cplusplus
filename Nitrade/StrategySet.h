#pragma once
#include "IStrategySet.h"
#include "PriceData.h"
#include "Strategy.h"
#include "IStrategyDefinition.h"
#include "OptimiseParameter.h"
namespace Nitrade{
	class StrategySet :
		public IStrategySet
	{
	private:
		std::unique_ptr<Strategy[]> _strategies;
		int _strategyCount{ 0 };
	public:
		StrategySet();
		virtual ~StrategySet();

		void createFrom(IStrategyDefinition* strategyDefintion);
		void run(IPriceData* dataSet);
	};
}

