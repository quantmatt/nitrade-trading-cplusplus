#pragma once

#include "PriceData.h"
#include "IStrategyDefinition.h"
namespace Nitrade {
	class IStrategySet
	{
	public:
		IStrategySet() = default;
		virtual ~IStrategySet() = default;
		
		virtual void createFrom(IStrategyDefinition* strategyDefintion) = 0;
		virtual void run(IPriceData* dataSet) = 0;
		virtual void init(ITradeManager* tradeManager, IAssetData* assetData) = 0;
	};
}

