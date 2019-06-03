#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"

using namespace Nitrade;
using namespace std;

class MockAssetData : public IAssetData {
public:


	MOCK_METHOD0(size, int());
	MOCK_METHOD1(bracketOp, IPriceData*(int index));

	IPriceData* operator [](int barOffset) override { return bracketOp(barOffset); }
};