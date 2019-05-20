#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"

using namespace Nitrade;
using namespace std;

class MockDataFactory : public IDataFactory {
public:

	// Inherited via IDataFactory
	MOCK_METHOD1(getAssetData, unique_ptr<IAssetData>(IStrategyDefinition* strategyDefinition));
	MOCK_METHOD1(getBinaryChunkReader, unique_ptr<IBinaryChunkReader>(const string dataPath));
	MOCK_METHOD2(getStrategySet, unique_ptr<IStrategySet>(IStrategyDefinition* strategyDefinition, IAsset* asset));
	MOCK_METHOD0(getTradeManager, unique_ptr<ITradeManager>());

};