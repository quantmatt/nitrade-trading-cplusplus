#pragma once
#include <iostream>
#include "TradeManager.h"
#include "DataManager.h"
#include "IAssetData.h"

namespace Nitrade {

	class Strategy
	{

	private:
		ITradeManager* _tradeManager{ nullptr };
		IAssetData* _assetData{ nullptr };
		IPriceData* _currentData{ nullptr };

	public:
		Strategy() {}
		virtual ~Strategy();

		
		void init(ITradeManager* tradeManager, IAssetData* assetDataa);
		
		//trade functions
		bool openTrade(std::string asset, int size, double stopLoss = 0, double takeProfit = 0);
		bool closeTrade(long tradeId);

		//helper functions to access the data
		float bidOpen(int offset = 0);
		float bidClose(int offset = 0);
		float bidHigh(int offset = 0);
		float bidLow(int offset = 0);				
		float askOpen(int offset = 0);
		float askClose(int offset = 0);
		float askHigh(int offset = 0);
		float askLow(int offset = 0);		
		int volume(int offset = 0);


		//returns true if this strategy runs on this priceData onBar event
		//also sets the current price data to process in OnBar
		bool setCurrentDataIfRequired(IPriceData* pd);

		virtual void onInit() {}; //these are setup by the child strategies
		virtual void onBar() {};//these are setup by the child strategies
	};
}
