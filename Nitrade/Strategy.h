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

	protected:
		ITradeManager* _tradeManager{ nullptr };
		IAssetData* _assetData{ nullptr };
		IPriceData* _currentData{ nullptr };

		std::map<std::string, double> _parameters;

		std::string _assetName{""};

		std::map<std::string, std::unique_ptr<Utils::ISeriesBuffer<double>>> _features;

	public:
		Strategy() {}
		virtual ~Strategy();

		virtual std::unique_ptr<Strategy> clone();

		void init(ITradeManager* tradeManager, IAssetData* assetData);

		void setParameter(std::string name, double value);
		double getParameter(std::string name);

		//trade functions
		bool openTrade(std::string asset, tradeDirection direction, int size, double stopLoss = 0, double takeProfit = 0);
		bool closeTrade(long tradeId);
		int getOpenTradeCount(std::string assetName, std::string strategyName);
		int getOpenTradeCount();

		//asset functions
		double getPip();

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