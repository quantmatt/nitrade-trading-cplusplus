#pragma once
#include <vector>
#include <map>
#include <iostream>
#include "utils.h"
#include "Structs.h"

namespace Nitrade {

	class ITradeManager
	{
	public:
		ITradeManager() {}
		virtual ~ITradeManager() {}

		virtual void openTrade(std::unique_ptr<Trade> trade) = 0;
		virtual void closeTrades(std::string asset, int id, tradeDirection direction, Bar* currentBar) = 0;
		virtual int getOpenTradeCount(std::string asset, int id) = 0;
		virtual bool writeTradesToBinary(std::string filepath) = 0;

	};


	//Trade Manager will have sub classes different for back testing and live trading
	class TradeManager :
		public ITradeManager
	{
	private:
		int _idCounter{ 1 }; //used for backtesting to create an id
		//trades are mapped by asset, variantId
		std::map<std::tuple<std::string, int>, std::vector<std::unique_ptr<Trade>>> _openTrades;
		std::map<std::tuple<std::string, int>, std::vector<std::unique_ptr<Trade>>> _closedTrades;

	public:
		TradeManager() {}
		virtual ~TradeManager() {}

		void openTrade(std::unique_ptr<Trade> trade);
		void closeTrades(std::string asset, int id, tradeDirection direction, Bar* currentBar);
		int getOpenTradeCount(std::string asset, int id);

		bool writeTradesToBinary(std::string filepath);
	};
}   
