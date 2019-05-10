#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"

using namespace Nitrade;
using namespace std;


class MockAsset : public IAsset {
public:

	//properties
	MOCK_METHOD0(getName, string());
	MOCK_METHOD0(hasBinaryReader, bool());
	MOCK_METHOD0(openFile, bool());
	MOCK_METHOD0(eof, bool());
	MOCK_METHOD0(endChunk, char*());
	MOCK_METHOD0(getChunk, char*());
	MOCK_METHOD0(closeFile, void());

	MOCK_METHOD2(addPriceData, void(string dataSetName, PriceData* pd));
	MOCK_METHOD0(getAllPriceData, IPriceData** ());

};