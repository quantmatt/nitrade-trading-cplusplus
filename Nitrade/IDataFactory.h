#pragma once

#include <iostream>
#include <memory>
#include "BinaryChunkReader.h"
#include "Asset.h"
#include "AssetData.h"
#include "StrategySet.h"
#include "IStrategyDefinition.h"

namespace Nitrade {
	class IDataFactory
	{
	public:
		IDataFactory() = default;
		virtual ~IDataFactory() = default;

		virtual std::unique_ptr<IAsset> getAsset(const std::string assetName) = 0;
		virtual std::unique_ptr<IAssetData> getAssetData(IStrategyDefinition* strategyDefinition) = 0;
		virtual std::unique_ptr<IBinaryChunkReader> getBinaryChunkReader(const std::string dataPath) = 0;
		virtual std::unique_ptr<IStrategySet> getStrategySet(IStrategyDefinition* strategyDefinition, IAsset* asset) = 0;
	};
}

