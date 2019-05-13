#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"

using namespace Nitrade;
using namespace std;

class MockPriceData : public IPriceData {
public:

	
	MOCK_METHOD1(updateCurrentBarFromBar, bool(Bar* newInfo));
	MOCK_METHOD2(updateCurrentBarFromTick, bool(float quote, bool isBid));
	MOCK_METHOD1(bracketOp, Bar*(int index));
	MOCK_METHOD0(getName, string());

	Bar* operator [](int barOffset) override { return bracketOp(barOffset); }
};