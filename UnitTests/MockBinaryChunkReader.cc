#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"

using namespace Nitrade;
using namespace std;

class MockBinaryChunkReader : public IBinaryChunkReader {
public:

	MOCK_METHOD0(isOpen, bool());
	MOCK_METHOD0(openFile, bool());
	MOCK_METHOD0(eof, bool());
	MOCK_METHOD0(endChunk, char* ());
	MOCK_METHOD0(getChunk, char* ());
	MOCK_METHOD0(closeFile, void());


};