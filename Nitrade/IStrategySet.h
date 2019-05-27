#pragma once

#include "PriceData.h"
#include "Strategy.h"

namespace Nitrade {
	class IStrategySet
	{
	public:
		IStrategySet() = default;
		virtual ~IStrategySet() = default;
		
		virtual void createFrom(Strategy* strategy, IAsset* asset) = 0;
		virtual void run(IPriceData* dataSet) = 0;
		virtual void init(ITradeManager* tradeManager, IAssetData* assetData) = 0;
		virtual std::vector<std::unique_ptr<StrategyKey>> getStrategyKeys() = 0;
	};
}

