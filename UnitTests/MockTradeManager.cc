#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"

using namespace Nitrade;
using namespace std;

class MockTradeManager : public ITradeManager {

public:

	MOCK_METHOD1(updateOpenTrades, void(Bar* bar));
	MOCK_METHOD1(openTrade, void(unique_ptr<Trade> trade));
	MOCK_METHOD4(closeTrades, void(string asset, int id, tradeDirection direction, Bar* currentBar));
	MOCK_METHOD4(closeTrade, void(std::string asset, int id, long long tradeId, Bar* currentBar));
	MOCK_METHOD3(closeCurrentTrade, std::vector<std::unique_ptr<Trade>>::iterator(std::vector<std::unique_ptr<Trade>>::iterator trade, Bar* currentBar, std::tuple<std::string, int> key));
	MOCK_METHOD2(getOpenTradeCount, int(string asset, int id));
	MOCK_METHOD1(writeTradesToBinary, bool(string filepath));
	MOCK_METHOD0(loadAssetDetails, void());
	MOCK_METHOD1(getAsset, IAsset* (string assetName));


};