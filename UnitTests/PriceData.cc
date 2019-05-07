#pragma once
#include "pch.h"
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "../NitradeTradeConsole/IPriceData.h"
#include "../NitradeTradeConsole/Structs.h"

using namespace Nitrade;
using namespace std;

class MockPriceData : public IPriceData {
public:

	MOCK_METHOD3(init, void(int lookback, int barSize, int arraySizeFactor));
	MOCK_METHOD1(updateCurrentBarFromBar, bool(Bar* newInfo));
	MOCK_METHOD2(updateCurrentBarFromTick, bool(float quote, bool isBid));


};