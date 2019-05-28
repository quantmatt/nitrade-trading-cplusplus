#pragma once
#include "PriceData.h"
#include "Strategy.h"
#include "OptimiseParameter.h"
#include "vector"

namespace Nitrade{

	class IStrategySet
	{
	public:
		IStrategySet() = default;
		virtual ~IStrategySet() = default;

		virtual void createFrom(Strategy* strategy, IAsset* asset) = 0;
		virtual void run(IPriceData* dataSet) = 0;
		virtual void init(ITradeManager* tradeManager, IAssetData* assetData) = 0;
		virtual std::vector<std::unique_ptr<StrategyKey>> getStrategyKeys() = 0;

		virtual Strategy* operator [](int index) = 0;
	};

	class StrategySet :
		public IStrategySet
	{
	private:
		std::vector<std::unique_ptr<Strategy>> _strategies;
		int _strategyCount{ 0 };
	public:
		StrategySet();
		virtual ~StrategySet();

		void createFrom(Strategy* baseStrategy, Nitrade::IAsset* asset);
		void init(ITradeManager* tradeManager, IAssetData* assetData);
		void run(IPriceData* dataSet);
		std::vector<std::unique_ptr<StrategyKey>> getStrategyKeys();

		Strategy* operator [](int index) { return _strategies[index].get(); }
	};
}

