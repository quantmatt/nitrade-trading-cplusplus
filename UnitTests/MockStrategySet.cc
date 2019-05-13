#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"

using namespace Nitrade;
using namespace std;
using namespace Utils;

class MockStrategySet : public IStrategySet {

	MOCK_METHOD1(run, void(const IPriceData* dataSet));
};