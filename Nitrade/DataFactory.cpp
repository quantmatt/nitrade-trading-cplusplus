#include "DataFactory.h"

std::unique_ptr<Nitrade::IAsset> Nitrade::DataFactory::getAsset(const std::string assetName)
{
	return std::make_unique<Asset>(assetName);
}

std::unique_ptr<Nitrade::IAssetData> Nitrade::DataFactory::getAssetData(IStrategyDefinition* strategyDefinition)
{
	//use the strategyDefinition data set parameters to create a new PriceData object for each of the datasets required
	//by this strategy
	int dataSetCount = strategyDefinition->getDataSetCount();

	auto assetData = std::make_unique<AssetData>(dataSetCount);
	for (int i = 0; i < dataSetCount; i++)
	{
		std::tuple<std::string, int, int> dataSetParams = strategyDefinition->getDataSetParams(i);
		assetData->set(i, std::get<0>(dataSetParams), std::get<1>(dataSetParams), std::get<2>(dataSetParams));
	}

	return std::move(assetData);

}

std::unique_ptr<Nitrade::IBinaryChunkReader> Nitrade::DataFactory::getBinaryChunkReader(const std::string dataPath)
{
	return std::make_unique<BinaryChunkReader>(dataPath);
}

std::unique_ptr<Nitrade::IStrategySet> Nitrade::DataFactory::getStrategySet(IStrategyDefinition* strategyDefinition,
	IAsset* asset)
{
	auto strategySet = std::make_unique<StrategySet>();
	strategySet->createFrom(strategyDefinition, asset->getName());
	return strategySet;
}

std::unique_ptr<Nitrade::ITradeManager> Nitrade::DataFactory::getTradeManager()
{
	return std::make_unique<TradeManager>();
}
