#pragma once
#include <iostream>
#include "TradeManager.h"
#include "DataManager.h"

namespace Nitrade {

	class Strategy
	{

	private:
		ITradeManager* _tradeManager{ nullptr };
		IDataManager* _dataManager{ nullptr };

	public:
		Strategy(ITradeManager* tradeManager, IDataManager* dataManager) :
			_tradeManager(tradeManager), _dataManager(dataManager) {};
		virtual ~Strategy();

		//trade functions
		bool openTrade(std::string asset, int size, double stopLoss = 0, double takeProfit = 0);
		bool closeTrade(long tradeId);

		//helper functions to access the data
		float bidOpen(int offset = 0);
		float bidClose(int offset = 0);
		float bidHigh(int offset = 0);
		float bidLow(int offset = 0);		
		float bidOpen(std::string asset, int offset = 0);
		float bidClose(std::string asset, int offset = 0);
		float bidHigh(std::string asset, int offset = 0);
		float bidLow(std::string asset, int offset = 0);		
		float bidOpen(std::string asset, std::string dataset, int offset = 0);
		float bidClose(std::string asset, std::string dataset, int offset = 0);
		float bidHigh(std::string asset, std::string dataset, int offset = 0);
		float bidLow(std::string asset, std::string dataset, int offset = 0);		
		float askOpen(int offset = 0);
		float askClose(int offset = 0);
		float askHigh(int offset = 0);
		float askLow(int offset = 0);
		float askOpen(std::string asset, int offset = 0);
		float askClose(std::string asset, int offset = 0);
		float askHigh(std::string asset, int offset = 0);
		float askLow(std::string asset, int offset = 0);
		float askOpen(std::string asset, std::string dataset, int offset = 0);
		float askClose(std::string asset, std::string dataset, int offset = 0);
		float askHigh(std::string asset, std::string dataset, int offset = 0);
		float askLow(std::string asset, std::string dataset, int offset = 0);
		int volume(int offset = 0);
		int volume(std::string asset, int offset = 0);
		int volume(std::string asset, std::string dataset, int offset = 0);

		virtual void onInit() = 0; //these are setup by the child strategies
		virtual void onBar() = 0;//these are setup by the child strategies
	};
}
