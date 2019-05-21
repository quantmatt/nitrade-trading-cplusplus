#include "TradeManager.h"
#include <algorithm>


void Nitrade::TradeManager::updateOpenTrades(Bar* bar)
{
	for (auto& tradeSet : _openTrades)
	{
		for (auto trade = tradeSet.second.begin(); trade != tradeSet.second.end();)
		{
			//update the current price level
			if ((*trade)->direction == tradeDirection::Long)
				(*trade)->closeLevel = bar->bidClose;
			else
				(*trade)->closeLevel = bar->askClose;

			//calculate profit	
			(*trade)->profit = (*trade)->calcProfit();

			//Work out if any stops or targets have been hit
			bool stopHit = false;
			bool targetHit = false;			
			if ((*trade)->direction == tradeDirection::Short)
			{
				if (bar->askLow <= (*trade)->takeProfitLevel)
					targetHit = true;
				else if (bar->askHigh >= (*trade)->stopLevel)
					stopHit = true;
			}
			else
			{
				if (bar->bidHigh >= (*trade)->takeProfitLevel)
					targetHit = true;
				else if (bar->bidLow <= (*trade)->stopLevel)
					stopHit = true;
			}

			//check for stops first (conservative)
			//NOTE: if stop and target are both hit in same minute bar then need tick data to see which came first
			if (stopHit)
			{
				float slippage = 0;//estimate slippage
				(*trade)->exit = exitType::StopLoss;
				(*trade)->closeLevel = (*trade)->stopLevel + slippage;
				//update the trade iterator because closing a trade changes the openTrades vector
				trade = closeCurrentTrade(trade, bar, tradeSet.first);
			}
			//and targets
			else if (targetHit)
			{

				float slippage = 0;//estimate slippage
				(*trade)->exit = exitType::TakeProfit;
				(*trade)->closeLevel = (*trade)->takeProfitLevel + slippage;
				//update the trade iterator because closing a trade changes the openTrades vector
				trade = closeCurrentTrade(trade, bar, tradeSet.first);
			}
			else
				trade++; //move the incrementer

		}
	}

}



void Nitrade::TradeManager::openTrade(std::unique_ptr<Trade> trade, std::map<std::string, double>& data)
{
	//get the details from the asset needed to calculate profit
	auto assetDetails = getAsset(trade->assetName);
	trade->pip = assetDetails->getPip();
	trade->pipCost = assetDetails->getPipCost();

	//set the stop and target levels
	if (trade->direction == tradeDirection::Short)
	{
		if(trade->stopLoss > 0)
			trade->stopLevel = trade->openLevel + trade->stopLoss;
		if(trade->takeProfit > 0)
			trade->takeProfitLevel = trade->openLevel - trade->takeProfit;
	}
	else
	{
		if (trade->stopLoss > 0)
			trade->stopLevel = trade->openLevel - trade->stopLoss;
		if (trade->takeProfit > 0)
			trade->takeProfitLevel = trade->openLevel + trade->takeProfit;
	}

	//set the trade id
	trade->tradeId = _idCounter++;

	//save the trade data associated with this trade if there is any.
	if (data.size() > 0)
		_dataManager.push_back(trade->tradeId, data);

	//put opentrades in a map for fast referencing, key is asset and variantid
	_openTrades[std::make_tuple(trade->assetName, trade->variantId)].push_back(std::move(trade));
}

void Nitrade::TradeManager::closeTrades(std::string asset, int id, tradeDirection direction, Bar* currentBar)
{
	
	//get the details from the asset needed to calculate profit
	auto assetDetails = getAsset(asset);
	float pip = assetDetails->getPip();
	float pipCost = assetDetails->getPipCost();
	float commission = assetDetails->getCommission();

	//loop through all open trades and close any that match the passed direction
	auto key = std::make_tuple(asset, id);	
	for (auto trade = _openTrades[key].begin(); trade != _openTrades[key].end();)
	{			
		//only close trades that match the direction passed to this function
		if ((*trade)->direction == direction)
		{
			//set the close level depending on the direction of the trade
			//long trades need to sell at the bid and short trades need to buy at the ask to cover
			if ((*trade)->direction == tradeDirection::Long)
				(*trade)->closeLevel = currentBar->bidOpen;
			else
				(*trade)->closeLevel = currentBar->askOpen;

			//closeCurrentTrade will update and return the next iterator
			trade = closeCurrentTrade(trade, currentBar, key);			
		}
		//opentrades vector has not been modified so increment the iterator
		else
			trade++;
	}

}

void Nitrade::TradeManager::closeTrade(std::string asset, int id, long long tradeId, Bar* currentBar)
{
	
	//loop through all open trades and close any that match the passed direction
	auto key = std::make_tuple(asset, id);
	for (auto trade = _openTrades[key].begin(); trade != _openTrades[key].end();)
	{
		//only close the trade that matches this tradeId
		if ((*trade)->tradeId == tradeId)
		{
			//set the close level depending on the direction of the trade
			//long trades need to sell at the bid and short trades need to buy at the ask to cover
			if ((*trade)->direction == tradeDirection::Long)
				(*trade)->closeLevel = currentBar->bidOpen;
			else
				(*trade)->closeLevel = currentBar->askOpen;

			closeCurrentTrade(trade, currentBar, key);
			break; //only one trade should match the tradeId
		}
		//opentrades vector has not been modified so increment the iterator
		else
			trade++;
	}

}

std::vector<std::unique_ptr<Nitrade::Trade>>::iterator Nitrade::TradeManager::closeCurrentTrade(std::vector<std::unique_ptr<Trade>>::iterator trade, 
	Bar* currentBar,
	std::tuple<std::string, int> key)
{
	//trade iterator is passed by value because it is a cheap copy

	//get the details from the asset needed to calculate profit
	auto assetDetails = getAsset(std::get<0>(key));
	float pip = assetDetails->getPip();
	float pipCost = assetDetails->getPipCost();
	float commission = assetDetails->getCommission();
	
	//set the close time
	(*trade)->closeTime = currentBar->timestamp;
	

	//add trading costs
	(*trade)->commission = commission;

	//calculate profit	
	(*trade)->profit = (*trade)->calcProfit();

	//add to the closed trades array and transfer ownership
	_closedTrades[key].push_back(std::move((*trade)));

	//remove from open trades and set the iterator to the next trade
	trade = _openTrades[key].erase(trade);

	return trade;

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

bool Nitrade::TradeManager::writeTradeDataToBinary(std::string filepath)
{
	//writes the addition data that was recorded when each trade was open
	return _dataManager.toBinary(filepath);
}

void Nitrade::TradeManager::loadAssetDetails()
{
	//load all the asset details from the Assets.csv
	_loadedAssets = Utils::IOIterator::vectorFromCsv<Asset>(std::string(DEFAULT_ASSET_DETAILS_CSV), true);
}


Nitrade::IAsset* Nitrade::TradeManager::getAsset(std::string assetName)
{
	//try to find the asset in the loaded assets
	for (auto& asset : _loadedAssets)
	{
		//create a string of size 10 with right side buffered with white space
		//since the Asset class has a char arary of size 10 for the assetName
		assetName.resize(9, ' ');
		//return a reference to the asset if found
		if (asset->getName() == assetName)
			return asset.get();

	}

	//throw an error if the asset can't be found
	std::string err = assetName + " does not exist in loaded Asset list.";
	throw std::exception(err.c_str());
}

