#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "NitradeLib.h"
#include "MockStrategyDefinition.cc"
#include "MockDataFactory.cc"
#include "MockAsset.cc"
#include "MockBinaryChunkReader.cc"
#include "MockAssetData.cc"
#include "MockPriceData.cc"
#include "MockStrategySet.cc"

using namespace Nitrade;
using namespace std;
using namespace testing;


class HistoricSimulatorTests : public ::testing::Test
{
protected:

	unique_ptr<MockStrategyDefinition> sd = make_unique<MockStrategyDefinition>();
	unique_ptr<MockDataFactory> df = make_unique<MockDataFactory>();
	unique_ptr<MockAsset> mockAsset = make_unique<MockAsset>();	
	unique_ptr<MockStrategySet> mockStrategySet = make_unique<MockStrategySet>(); 
	unique_ptr<MockAssetData> mockAssetData = make_unique<MockAssetData>();
	unique_ptr<MockPriceData> mockPriceData = make_unique<MockPriceData>();
	unique_ptr<MockBinaryChunkReader> mockBinaryChunkReader = make_unique<MockBinaryChunkReader>();

	HistoricSimulator hs;
	const string binDataPath = "D:\\TickData\\EURUSD_m1.bin";
	const int threadCount = 12;


	virtual void SetUp() {
		
		
	}

	virtual void TearDown() {

	}

	unique_ptr<Bar[]> getBars(int size, bool corrupt)
	{
		long long startTime = 1557210984000000000ll;
		if (corrupt)
			startTime = -1000ll;

		unique_ptr<Bar[]> bars = make_unique<Bar[]>(size);

		for (int i = 0; i < size; i++)
			//adds 10 minutes onto the timestamp on each iteration
			bars[i] = Bar{ startTime + i * 600000000000, 1.0001f,1.0002f, 1.0003f,1.0001f,1.0005f,1.0006f,1.0007f,1.0005f, 5 };

		return bars;
	}

};
TEST_F(HistoricSimulatorTests, OptimiseWithThreads) {

	hs.Optimise(threadCount);
}

TEST_F(HistoricSimulatorTests, OptimiseWithAssetFailAssetDetailsLoad) {
	
	EXPECT_CALL(*df, getAsset(_)).WillRepeatedly(Throw(exception()));
	hs.Setup(move(sd), move(df));

	EXPECT_THROW(hs.Optimise(""), exception);
}

TEST_F(HistoricSimulatorTests, OptimiseWithAssetStrategySetFail) {

	EXPECT_CALL(*df, getAsset(_)).WillRepeatedly(Return(ByMove(move(mockAsset))));
	EXPECT_CALL(*df, getStrategySet(_, _)).WillRepeatedly(Throw(exception()));
	hs.Setup(move(sd), move(df));

	EXPECT_THROW(hs.Optimise(""), exception);
}

TEST_F(HistoricSimulatorTests, OptimiseWithAssetGetAssetDataFail) {

	EXPECT_CALL(*df, getAsset(_)).WillRepeatedly(Return(ByMove(move(mockAsset))));
	EXPECT_CALL(*df, getStrategySet(_, _)).WillRepeatedly(Return(ByMove(move(mockStrategySet))));
	EXPECT_CALL(*df, getAssetData(_)).WillRepeatedly(Throw(exception()));
	hs.Setup(move(sd), move(df));

	EXPECT_THROW(hs.Optimise(""), exception);
}

TEST_F(HistoricSimulatorTests, OptimiseWithAssetGetBinaryReaderFail) {

	EXPECT_CALL(*df, getAsset(_)).WillRepeatedly(Return(ByMove(move(mockAsset))));
	EXPECT_CALL(*df, getStrategySet(_, _)).WillRepeatedly(Return(ByMove(move(mockStrategySet))));
	EXPECT_CALL(*df, getAssetData(_)).WillRepeatedly(Return(ByMove(move(mockAssetData))));
	EXPECT_CALL(*df, getBinaryChunkReader(_)).WillRepeatedly(Throw(exception()));
	hs.Setup(move(sd), move(df));

	EXPECT_THROW(hs.Optimise(""), exception);
}

TEST_F(HistoricSimulatorTests, OptimiseWithAssetBinaryDataInvalid) {

	EXPECT_CALL(*mockAsset, getDataPath()).WillRepeatedly(Return(binDataPath));

	//permits entry into binary reader loop
	EXPECT_CALL(*mockBinaryChunkReader, eof())
		.WillOnce(Return(false));

	EXPECT_CALL(*df, getAsset(_)).WillRepeatedly(Return(ByMove(move(mockAsset))));
	EXPECT_CALL(*df, getStrategySet(_, _)).WillRepeatedly(Return(ByMove(move(mockStrategySet))));
	EXPECT_CALL(*df, getAssetData(_)).WillRepeatedly(Return(ByMove(move(mockAssetData))));
	EXPECT_CALL(*df, getBinaryChunkReader(_)).WillRepeatedly(Return(ByMove(move(mockBinaryChunkReader))));
	hs.Setup(move(sd), move(df));

	EXPECT_THROW(hs.Optimise(""), invalid_argument);
}

TEST_F(HistoricSimulatorTests, OptimiseWithAssetWillSucceed) {

	EXPECT_CALL(*mockAsset, getDataPath()).WillRepeatedly(Return(binDataPath));

	
	EXPECT_CALL(*mockBinaryChunkReader, eof())
		.WillOnce(Return(false))
		.WillOnce(Return(true));

	int size = 20;
	unique_ptr<Bar[]> bars = getBars(size, false);

	//setup the start and end of the mock binary reader based on the simulted bars above
	char* start = (char*)& bars[0];
	char* end = start + sizeof(Bar) * size;

	EXPECT_CALL(*mockBinaryChunkReader, getChunk()).WillOnce(Return(start));
	EXPECT_CALL(*mockBinaryChunkReader, endChunk()).WillOnce(Return(end));

	EXPECT_CALL(*mockAssetData, size()).WillOnce(Return(1));
	EXPECT_CALL(*mockAssetData, bracketOp(_)).WillRepeatedly(Return(mockPriceData.get()));

	EXPECT_CALL(*mockPriceData, updateCurrentBarFromBar(_)).WillRepeatedly(Return(false));

	EXPECT_CALL(*df, getAsset(_)).WillRepeatedly(Return(ByMove(move(mockAsset))));
	EXPECT_CALL(*df, getStrategySet(_, _)).WillRepeatedly(Return(ByMove(move(mockStrategySet))));
	EXPECT_CALL(*df, getAssetData(_)).WillRepeatedly(Return(ByMove(move(mockAssetData))));
	EXPECT_CALL(*df, getBinaryChunkReader(_)).WillRepeatedly(Return(ByMove(move(mockBinaryChunkReader))));
	hs.Setup(move(sd), move(df));

	


	hs.Optimise("");
}
