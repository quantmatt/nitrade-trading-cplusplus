#pragma once

#include <iostream>
#include <memory>
#include <tuple>
#include "BinaryChunkReader.h"
#include "Asset.h"
#include "AssetData.h"
#include "StrategySet.h"

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

}

