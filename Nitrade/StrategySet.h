#pragma once
#include "IStrategySet.h"
#include "PriceData.h"
#include "Strategy.h"
#include "IStrategyDefinition.h"
#include "OptimiseParameter.h"
#include "vector"

namespace Nitrade{
	class StrategySet :
		public IStrategySet
	{
	private:
		std::vector<std::unique_ptr<Strategy>> _strategies;
		int _strategyCount{ 0 };
	public:
		StrategySet();
		virtual ~StrategySet();

		void createFrom(IStrategyDefinition* strategyDefintion, Nitrade::Asset* asset);
		void init(ITradeManager* tradeManager, IAssetData* assetData);
		void run(IPriceData* dataSet);
	};
}

