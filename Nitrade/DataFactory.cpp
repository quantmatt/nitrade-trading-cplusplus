#include "DataFactory.h"


std::unique_ptr<Nitrade::IAssetData> Nitrade::DataFactory::getAssetData(Strategy* strategy)
{
	//use the strategyDefinition data set parameters to create a new PriceData object for each of the datasets required
	//by this strategy
	int dataSetCount = strategy->getDataSetCount();

	auto assetData = std::make_unique<AssetData>(dataSetCount);
	for (int i = 0; i < dataSetCount; i++)
	{
		//the strategy definition holds the required details for the dataset in a tuple
		//with assetname, lookback period and bar size in minutes
		std::tuple<std::string, int, int> dataSetParams = strategy->getDataSetParams(i);

		//creates a new Price data object and adds it to the array of price data objects
		//for this asset
		assetData->set(i, std::get<0>(dataSetParams), std::get<1>(dataSetParams), std::get<2>(dataSetParams));
	}

	return std::move(assetData);

}

std::unique_ptr<Nitrade::IBinaryChunkReader> Nitrade::DataFactory::getBinaryChunkReader(const std::string dataPath)
{
	return std::make_unique<BinaryChunkReader>(dataPath);
}

std::unique_ptr<Nitrade::IStrategySet> Nitrade::DataFactory::getStrategySet(Strategy* strategy,
	Nitrade::IAsset* asset)
{
	auto strategySet = std::make_unique<StrategySet>();
	strategySet->createFrom(strategy, asset);
	return strategySet;
}

std::unique_ptr<Nitrade::ITradeManager> Nitrade::DataFactory::getTradeManager()
{
	return std::make_unique<TradeManager>();
}
