#pragma once
#include "DataFactory.h"
#include "Strategy.h"
#include "AssetData.h"
#include "AssetData.h"
#include "StrategySet.h"
#include "StrategySet.h"
#include "BinaryChunkReader.h"

namespace Nitrade {
	class IDataFactory
	{
	public:
		IDataFactory() = default;
		virtual ~IDataFactory() = default;

		virtual std::unique_ptr<IAssetData> getAssetData(Strategy* strategy) = 0;
		virtual std::unique_ptr<IBinaryChunkReader> getBinaryChunkReader(const std::string dataPath) = 0;
		virtual std::unique_ptr<IStrategySet> getStrategySet(Strategy* strategy, IAsset* asset) = 0;
		virtual std::unique_ptr<ITradeManager> getTradeManager() = 0;
	};

	class DataFactory :
		public IDataFactory
	{
	public:
		DataFactory() = default;
		virtual ~DataFactory() = default;

		// Inherited via IDataFactory
		virtual std::unique_ptr<IAssetData> getAssetData(Strategy* strategy) override;
		virtual std::unique_ptr<IBinaryChunkReader> getBinaryChunkReader(const std::string dataPath) override;
		virtual std::unique_ptr<IStrategySet> getStrategySet(Strategy* strategy, IAsset* asset) override;
		virtual std::unique_ptr<ITradeManager> getTradeManager() override;
	};
}

