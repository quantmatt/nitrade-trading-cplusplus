#include "TradeManager.h"
#include <algorithm>
void Nitrade::TradeManager::openTrade(std::unique_ptr<Trade> trade)
{
	//set the trade id
	trade->tradeId = _idCounter++;
	//put opentrades in a map for fast referencing, key is asset and variantid
	_openTrades[std::make_tuple(trade->assetName, trade->variantId)].push_back(std::move(trade));
}

void Nitrade::TradeManager::closeTrades(std::string asset, int id, tradeDirection direction, Bar* currentBar)
{
	
	//get the details from the asset needed to calculate profit
	auto assetDetails = getAsset(asset);
	float pip = assetDetails.getPip();
	float pipCost = assetDetails.getPipCost();
	float commission = assetDetails.getCommission();

	//loop through all open trades and close any that match the passed direction
	auto key = std::make_tuple(asset, id);	
	for (auto trade = _openTrades[key].begin(); trade != _openTrades[key].end();)
	{			
		//only close trades that match the direction passed to this function
		if ((*trade)->direction == direction)
		{
			//set the close time
			(*trade)->closeTime = currentBar->timestamp;

			//set the close level depending on the direction of the trade
			//long trades need to sell at the bid and short trades need to buy at the ask to cover
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

			//remove from open trades and set the iterator to the next trade
			trade = _openTrades[key].erase(trade);
		}
		//opentrades vector has not been modified so increment the iterator
		else
			trade++;
	}

}

int Nitrade::TradeManager::getOpenTradeCount(std::string asset, int id)
{
	//returns the number of open trades for this asset and variant id
	return _openTrades[std::make_tuple(asset, id)].size();
}

bool Nitrade::TradeManager::writeTradesToBinary(std::string filepath)
{
	//writes all trades to binary from every asset and variant id as one bid trades file

	//loop through every asset and variant id trade result set
	std::vector<std::unique_ptr<Trade>> allTrades;
	for (auto& vec : _closedTrades)
	{
		for (auto& trade : vec.second)
		{
			//copy the trade object into a new variable
			allTrades.push_back(std::make_unique<Trade>(*trade));
		}
	}

	//write to binary file
	return Utils::IOIterator::binary<Trade>(filepath, allTrades);	
}

void Nitrade::TradeManager::loadAssetDetails()
{
	//load all the asset details from the Assets.csv
	_loadedAssets = Utils::IOIterator::vectorFromCsv<Asset>(std::string(DEFAULT_ASSET_DETAILS_CSV), true);
}


Nitrade::Asset& Nitrade::TradeManager::getAsset(std::string assetName)
{
	//try to find the asset in the loaded assets
	for (auto& asset : _loadedAssets)
	{
		//create a string of size 10 with right side buffered with white space
		//since the Asset class has a char arary of size 10 for the assetName
		assetName.resize(9, ' ');
		//return a reference to the asset if found
		if (asset->getName() == assetName)
			return *asset;

	}

	//throw an error if the asset can't be found
	std::string err = assetName + " does not exist in loaded Asset list.";
	throw std::exception(err.c_str());
}
