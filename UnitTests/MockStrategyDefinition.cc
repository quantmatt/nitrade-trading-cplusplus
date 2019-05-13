#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"

using namespace Nitrade;
using namespace std;

class MockStrategyDefinition : public IStrategyDefinition {

	MOCK_METHOD0(getAssetNames, unique_ptr<string[]>());
	MOCK_METHOD1(getDataSetParams, std::tuple<std::string, int, int>& (int index));
	MOCK_METHOD0(getDataSetCount, int());
};