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
