#pragma once
#include <iostream>
#include "TradeManager.h"
#include "DataManager.h"
#include "FastAccessDynamicArray.h"

namespace Nitrade {

	class Strategy
	{

	private:
		ITradeManager* _tradeManager{ nullptr };
		IDataManager* _dataManager{ nullptr };
		Utils::FastAccessDynamicArray<IPriceData*> _requiredData;
		IPriceData* _currentData{ nullptr };

	public:
		Strategy() {}
		virtual ~Strategy();

		
		void init(ITradeManager* tradeManager, IDataManager* dataManager);
		//needs to be called after strategy init so the pointers from dataManager can be found
		//this is to quickly determine if the onBar event should be run for this strategy since onBar events
		//can be triggere by any asset or any dataset
		void addRequireData(std::string assetName, std::string datasetName);


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

		//returns true if this strategy runs on this priceData onBar event
		//also sets the current price data to process in OnBar
		bool setCurrentDataIfRequired(IPriceData* pd);

		virtual void onInit() {}; //these are setup by the child strategies
		virtual void onBar() {};//these are setup by the child strategies
	};
}
