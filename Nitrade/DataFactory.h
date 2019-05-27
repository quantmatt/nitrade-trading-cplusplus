#pragma once
#include "IDataFactory.h"

namespace Nitrade {
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

