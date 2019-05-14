#pragma once
#include <iostream>
#include <memory>
#include <map>
#include "TradeManager.h"
#include "IAssetData.h"
#include "PriceData.h"
#include "SeriesBuffer.h"
#include "Indicators.h"

namespace Nitrade {

	class Strategy
	{

	private:
		IAssetData* _assetData{ nullptr };
		IPriceData* _currentData{ nullptr };
		ITradeManager* _tradeManager{ nullptr };
	
	protected:
		std::map<std::string, double> _parameters;
		int _variantId{ 0 }; //used to identify the strategy variant ie. different parameter values
		std::string _assetName{""};

		std::map<std::string, std::unique_ptr<Utils::ISeriesBuffer<double>>> _features;

	public:
		Strategy() {}
		virtual ~Strategy();

		virtual std::unique_ptr<Strategy> clone();

		void init(ITradeManager* tradeManager, IAssetData* assetData);

		void setParameter(std::string name, double value);
		double getParameter(std::string name);

		void setVariantId(int id) { _variantId = id; }
		int getVariantId() {return _variantId; }
		void setAssetName(std::string name) { _assetName = name; }
		std::string getAssetName() { return _assetName; }

		//trade functions
		bool openTrade(tradeDirection direction, int size, double stopLoss = 0, double takeProfit = 0);
		bool closeTrade(int tradeId);
		void closeTrades(tradeDirection direction);
		int getOpenTradeCount(std::string assetName, std::string strategyName);
		int getOpenTradeCount();
		
		int getBarIndex() { return _currentData->getBarIndex(); }
		std::string getDatasetName() { return _currentData->getName(); }

		//asset functions
		double getPip();

		//functions to access the data - will always add 1 to the offset to get the data from the last closed bar
		//data from the current incomplete bar should only be accessed in an ontick function
		float bidOpen(unsigned int offset = 0u);
		float bidClose(unsigned int offset = 0u);
		float bidHigh(unsigned int offset = 0u);
		float bidLow(unsigned int offset = 0u);
		float askOpen(unsigned int offset = 0u);
		float askClose(unsigned int offset = 0u);
		float askHigh(unsigned int offset = 0u);
		float askLow(unsigned int offset = 0u);
		int volume(unsigned int offset = 0u);


		//returns true if this strategy runs on this priceData onBar event
		//also sets the current price data to process in OnBar
		bool setCurrentDataIfRequired(IPriceData* pd);

		virtual void onInit() {}; //these are setup by the child strategies
		virtual void onBar() {};//these are setup by the child strategies
	};
}