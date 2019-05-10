#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"

    
using namespace Nitrade;
using namespace std;

class MockController : public IController {
public:

	MOCK_METHOD1(hasBinaryReader, bool(string assetName));
	MOCK_METHOD1(openFile, bool(string assetName));
	MOCK_METHOD1(eof, bool(string assetName));
	MOCK_METHOD1(endChunk, char* (string assetName));
	MOCK_METHOD1(getChunk, char* (string assetName));
	MOCK_METHOD1(closeFile, void(string assetName));

	MOCK_METHOD1(addAsset, void(IAsset* asset));
	MOCK_METHOD1(getAsset, IAsset*(string assetName));
	MOCK_METHOD0(getAssetNames, vector<string>* ());

	MOCK_METHOD1(run, void(IPriceData* pd));

};