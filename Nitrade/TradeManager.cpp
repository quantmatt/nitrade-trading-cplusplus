#include "TradeManager.h"
#include <algorithm>
void Nitrade::TradeManager::openTrade(std::unique_ptr<Trade> trade)
{
	trade->tradeId = _idCounter++;
	_openTrades[std::make_tuple(trade->assetName, trade->variantId)].push_back(std::move(trade));
}

void Nitrade::TradeManager::closeTrades(std::string asset, int id, tradeDirection direction, Bar* currentBar)
{
	auto moveTrades = std::vector<std::unique_ptr<Trade>>();

	auto assetDetails = getAsset(asset);
	float pip = assetDetails.getPip();
	float pipCost = assetDetails.getPipCost();
	float commission = assetDetails.getCommission();

	auto key = std::make_tuple(asset, id);	
	for (auto trade = _openTrades[key].begin(); trade != _openTrades[key].end();)
	{			
		if ((*trade)->direction == direction)
		{
			(*trade)->closeTime = currentBar->timestamp;

			if (direction == tradeDirection::Long)
				(*trade)->closeLevel = currentBar->bidOpen;
			else
				(*trade)->closeLevel = currentBar->askOpen;

			//add trading costs
			(*trade)->commission = commission;

			//calculate profit
			float pointsProfit = (((*trade)->closeLevel - (*trade)->openLevel) * (*trade)->direction);
			float pipsProfit = (pointsProfit / pip);
			(*trade)->profit = ((pipsProfit * pipCost) - commission) * (*trade)->size;			
			
			//add to the closed trades array and transfer ownership
			_closedTrades[key].push_back(std::move((*trade)));

			trade = _openTrades[key].erase(trade);
		}
		else
			trade++;
	}

}

int Nitrade::TradeManager::getOpenTradeCount(std::string asset, int id)
{
	return _openTrades[std::make_tuple(asset, id)].size();
}

bool Nitrade::TradeManager::writeTradesToBinary(std::string filepath)
{
	std::vector<std::unique_ptr<Trade>> allTrades;
	for (auto& vec : _closedTrades)
	{
		for (auto& trade : vec.second)
		{
			//Trade tradeCopy = Trade(*trade);
			allTrades.push_back(std::make_unique<Trade>(*trade));
		}
	}

	return Utils::IOIterator::binary<Trade>(filepath, allTrades);	
}

void Nitrade::TradeManager::loadAssetDetails()
{
	_loadedAssets = Utils::IOIterator::vectorFromCsv<Asset>(std::string(DEFAULT_ASSET_DETAILS_CSV), true);
}


Nitrade::Asset& Nitrade::TradeManager::getAsset(std::string assetName)
{
	//try to find the asset in the loaded assets
	for (auto& asset : _loadedAssets)
	{
		//create a string of size 10 with right side buffered with white space
		//since the Asset class has a char arary of size 10 for the assetName
		assetName.resize(10, ' ');
		//return a reference to the asset if found
		if (asset->getName() == assetName)
			return *asset;

	}

	//throw an error if the asset can't be found
	std::string err = assetName + " does not exist in loaded Asset list.";
	throw std::exception(err.c_str());
}
