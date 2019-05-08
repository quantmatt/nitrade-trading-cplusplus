#include "pch.h"
#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../NitradeTradeConsole/BackTest.h"
#include "Controller.cc"
#include "PriceData.cc"

using namespace std;
using namespace Nitrade;
using namespace testing;


class BacktestTest : public ::testing::Test
{
protected:
	BackTest* bt{};

	Bar* bars{};
	Bar* bars2{};


	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
		if (bars != nullptr)
			delete[] bars;
		if (bars2 != nullptr)
			delete[] bars2;
	}


	Bar* getBars(int size, bool corrupt)
	{
		long long startTime = 1557210984000000000ll;
		if (corrupt)
			startTime = -1000ll;

		Bar* bars = new Bar[size];
		for (int i = 0; i < size; i++)
			//adds 10 minutes onto the timestamp on each iteration
			bars[i] = Bar{ startTime + i * 600000000000, 1.0001f,1.0002f, 1.0003f,1.0004f,1.0005f,1.0006f,1.0007f,1.0008f, 5 };

		return bars;
	}

	
};

TEST_F(BacktestTest, RunThrowsInvalidArgumentWhenPassedNullAndEmptyString) {

	ASSERT_THROW(bt->Run(NULL, ""), std::invalid_argument);
}

TEST_F(BacktestTest, RunThrowsInvalidArgumentWhenPassedNullAndString) {

	ASSERT_THROW(bt->Run(NULL, "EURUSD"), std::invalid_argument);
}


TEST_F(BacktestTest, RunThrowsInvalidArgumentWhenPassedControllerWithoutBinaryReader) {

	MockController controller;

	ASSERT_THROW(bt->Run(&controller, "EURUSD"), std::invalid_argument);
}

TEST_F(BacktestTest, RunThrowsInvalidArgumentWhenBinaryFileNotOpened) {


	MockController controller;
	EXPECT_CALL(controller, hasBinaryReader()).WillOnce(Return(true));
	EXPECT_CALL(controller, openFile()).WillOnce(Return(false));	
	ASSERT_THROW(bt->Run(&controller, "EURUSD"), std::invalid_argument);
}


TEST_F(BacktestTest, RunClosesFileOnComplete) {


	MockController controller;
	EXPECT_CALL(controller, hasBinaryReader()).WillOnce(Return(true));
	EXPECT_CALL(controller, openFile()).WillOnce(Return(true));
	
	//succesfully opened the binary file

	//Loop through zero chunks of mocked data
	EXPECT_CALL(controller, eof())
		.WillOnce(Return(true));

	EXPECT_CALL(controller, closeFile())
		.Times(Exactly(1));

	bt->Run(&controller, "EURUSD");

}

TEST_F(BacktestTest, RunProcessesMockBarDataCheckUpdateBarCount) {

	//create some mock price data to return from getAssetData
	MockPriceData pd;
	std::vector<IPriceData*>* priceData = new std::vector<IPriceData*>();
	priceData->push_back(&pd);


	MockController controller;
	//mock that a binaryReader exists
	EXPECT_CALL(controller, hasBinaryReader()).WillOnce(Return(true));

	//mock that the binary file opens successfully
	EXPECT_CALL(controller, openFile()).WillOnce(Return(true));

	//mock a PriceData object
	EXPECT_CALL(controller, getAssetData(_)).WillOnce(Return(priceData));

	//create some mock data
	int size = 10;
	bars = getBars(10, false);
	bars2 = getBars(10, false);

	//start chunk and end chunk will be the same so exit internal loop immediately
	//becuase we have no mocked data
	char* start = (char*)&bars[0];
	char* end = start + sizeof(Bar) * size;
	char* start2 = (char*)&bars2[0];
	char* end2 = start2 + sizeof(Bar) * size;
	
	EXPECT_CALL(controller, getChunk())
		.WillOnce(Return(start))
		.WillOnce(Return(start2));
	EXPECT_CALL(controller, endChunk())
		.WillOnce(Return(end))
		.WillOnce(Return(end2));
	
	//Loop through 2 chunks of mocked data
	EXPECT_CALL(controller, eof())
		.WillOnce(Return(false))
		.WillOnce(Return(false))
		.WillOnce(Return(true));

	
	EXPECT_CALL(pd, updateCurrentBarFromBar(_))
		.Times(Exactly(20))
		.WillRepeatedly(Return(false));

	bt->Run(&controller, "EURUSD");

}

TEST_F(BacktestTest, RunThrowBecauseBarDataInvalid) {

	//create some mock price data to return from getAssetData
	MockPriceData pd;
	std::vector<IPriceData*>* priceData = new std::vector<IPriceData*>();
	priceData->push_back(&pd);

	MockController controller;
	//mock that a binaryReader exists
	EXPECT_CALL(controller, hasBinaryReader()).WillOnce(Return(true));

	//mock that the binary file opens successfully
	EXPECT_CALL(controller, openFile()).WillOnce(Return(true));

	//mock a PriceData object
	EXPECT_CALL(controller, getAssetData(_)).WillOnce(Return(priceData));


	//create some mock data
	int size = 10;
	bars = getBars(size, false);
	bars2 = getBars(size, true);


	//start chunk and end chunk will be the same so exit internal loop immediately
	//becuase we have no mocked data
	char* start = (char*)& bars[0];
	char* end = start + sizeof(Bar) * size;
	char* start2 = (char*)& bars2[0];
	char* end2 = start2 + sizeof(Bar) * size;

	EXPECT_CALL(controller, getChunk())
		.WillOnce(Return(start))
		.WillOnce(Return(start2));
	EXPECT_CALL(controller, endChunk())
		.WillOnce(Return(end))
		.WillOnce(Return(end2));

	//Loop through 2 chunks of mocked data
	EXPECT_CALL(controller, eof())
		.WillOnce(Return(false))
		.WillOnce(Return(false));

	EXPECT_THROW(bt->Run(&controller, "EURUSD"), std::invalid_argument);


}