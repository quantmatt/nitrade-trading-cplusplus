#pragma once
#include <vector>
#include <map>
#include <iostream>
#include "utils.h"
#include "Asset.h"
#include "Structs.h"
#include "TradeDataManager.h"

namespace Nitrade {

	class ITradeManager
	{
	public:
		ITradeManager() = default;
		virtual ~ITradeManager() = default;

		virtual void updateOpenTrades(Bar* bar) = 0;
		virtual void openTrade(std::unique_ptr<Trade> trade, std::map<std::string, double>& data) = 0;
		virtual void closeTrades(std::string asset, int id, tradeDirection direction, Bar* currentBar) = 0;
		virtual void closeTrade(std::string asset, int id, long long tradeId, Bar* currentBar) = 0;
		virtual std::vector<std::unique_ptr<Trade>>::iterator closeCurrentTrade(std::vector<std::unique_ptr<Trade>>::iterator trade,
			Bar* currentBar, std::tuple<std::string, int> key) = 0;
		virtual int getOpenTradeCount(std::string asset, int id) = 0;
		virtual bool writeTradesToBinary(std::string filepath) = 0;
		virtual bool writeTradeDataToBinary(std::string filepath) = 0;
		virtual bool writeRunningPLToBinary(std::string filepath) = 0;

		virtual void loadAssetDetails() = 0;
		virtual IAsset* getAsset(std::string assetName) = 0;

		virtual void onDay(std::vector<std::unique_ptr<StrategyKey>>& keys, long long timestamp) = 0;

	};


	//Trade Manager will have sub classes different for back testing and live trading
	class TradeManager :
		public ITradeManager
	{
	private:
		int _idCounter{ 1 }; //used for backtesting to create an id
		//trades are mapped by asset, variantId
		std::map<StrategyKey, std::vector<std::unique_ptr<Trade>>> _openTrades;
		std::map<StrategyKey, std::vector<std::unique_ptr<Trade>>> _closedTrades;
		
		//map to hold the running PL for each strategy variant
		//held in a map in case future need to have the data available to the running strategy
		std::map<StrategyKey, std::vector<std::unique_ptr<RunningPL>>> _runningPL;

		std::vector<std::unique_ptr<Asset>> _loadedAssets;

		//holds the extra data for each trade and manages inserting new data, writing to binary
		TradeDataManager _dataManager;

	public:
		TradeManager() = default;
		virtual ~TradeManager() = default;

		void updateOpenTrades(Bar* bar);
		void openTrade(std::unique_ptr<Trade> trade, std::map<std::string, double>& data);
		void closeTrades(std::string asset, int id, tradeDirection direction, Bar* currentBar);
		std::vector<std::unique_ptr<Trade>>::iterator closeCurrentTrade(std::vector<std::unique_ptr<Trade>>::iterator trade,
			Bar* currentBar, std::tuple<std::string, int> key);
		void closeTrade(std::string asset, int id, long long tradeId, Bar* currentBar);
		int getOpenTradeCount(std::string asset, int id);

		bool writeTradesToBinary(std::string filepath);
		bool writeTradeDataToBinary(std::string filepath);
		bool writeRunningPLToBinary(std::string filepath);

		void loadAssetDetails();
		IAsset* getAsset(std::string assetName);
		void onDay(std::vector<std::unique_ptr<StrategyKey>>& keys, long long timestamp);

	

		
	};
}   
