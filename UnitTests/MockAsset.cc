#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"
#include <iostream>

using namespace Nitrade;
using namespace std;

class MockAsset : public IAsset {
public:

	//properties
	MOCK_CONST_METHOD0(getName, string());
	MOCK_CONST_METHOD0(getDataPath, string());
	MOCK_CONST_METHOD0(getPip, float());
	MOCK_CONST_METHOD0(getPoint, float());
	MOCK_CONST_METHOD0(getPipCost, float());
	MOCK_CONST_METHOD0(getCommission, float());



};