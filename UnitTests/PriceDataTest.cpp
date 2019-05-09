#include "gtest/gtest.h"
#include "NitradeLib.h"
using namespace std;
using namespace Nitrade;
using namespace Utils;

class PriceDataTest : public ::testing::Test
{
protected:
	PriceData pd = PriceData(200, 60);

	Bar* createBar(long long timestamp, float bo, float bc, float bh, float bl, float ao, float ac, float ah, float al, int vol)
	{
		Bar* bar = new Bar { timestamp, bo, bc, bh, bl, ao, ac, ah, al, vol};
		return bar;
	}
};

TEST_F(PriceDataTest, getBarReturnsOnEmptyReturnsNull) {
	EXPECT_EQ(pd[0], nullptr);
	EXPECT_EQ(pd[199], nullptr);
}

TEST_F(PriceDataTest, getBarOutsideIndexThrowsOutOfRange) {

	EXPECT_THROW(pd[200], out_of_range);
	EXPECT_THROW(pd[-10], out_of_range);
	EXPECT_THROW(pd[500], out_of_range);
}

TEST_F(PriceDataTest, addsNewBarEveryHour) {

	int size = 20;
	long long startTime = 1557210984000000000ll;	//12:40pm
	for (int i = 0; i < size; i++)
		//adds 10 minutes onto the timestamp on each iteration
		pd.updateCurrentBarFromBar(createBar(startTime + i * 600000000000, 1.0001f, 1.0002f, 1.0003f, 1.0004f, 1.0005f, 1.0006f, 1.0007f, 1.0008f, 5));
	

	EXPECT_EQ(pd[0]->bidOpen, 1.0001f);
	EXPECT_EQ(pd[1]->bidOpen, 1.0001f);
	EXPECT_EQ(pd[2]->bidOpen, 1.0001f);
	EXPECT_EQ(pd[3]->bidOpen, 1.0001f);
	EXPECT_EQ(pd[4], nullptr);
}

TEST_F(PriceDataTest, bidHighAfterSeveralBars) {

	long long startTime = 1557210984000000000ll; //12:40pm
	//adds 30 minutes onto the timestamp on each iteration
	int i = 0;
	//bar 1
	pd.updateCurrentBarFromBar(createBar(startTime + (i++ * 1800000000000), 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 1));
	//bar 2
	pd.updateCurrentBarFromBar(createBar(startTime + (i++ * 1800000000000), 1.0f, 2.0f, 3.0f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 1));
	pd.updateCurrentBarFromBar(createBar(startTime + (i++ * 1800000000000), 2.0f, 2.0f, 2.0f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 1));

	//remeber bars are in series so bar 0 is the most recently added bar
	EXPECT_NEAR(pd[0]->bidHigh, 3.0f, 0.01);
	EXPECT_NEAR(pd[1]->bidHigh, 1.0f, 0.01);
}

TEST_F(PriceDataTest, bidLowAfterSeveralBars) {

	long long startTime = 1557210984000000000ll; //12:40pm
	//adds 30 minutes onto the timestamp on each iteration
	int i = 0;
	//bar 1
	pd.updateCurrentBarFromBar(createBar(startTime + (i++ * 1800000000000), 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 1));
	//bar 2
	pd.updateCurrentBarFromBar(createBar(startTime + (i++ * 1800000000000), 1.0f, 2.0f, 3.0f, 1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 1));
	pd.updateCurrentBarFromBar(createBar(startTime + (i++ * 1800000000000), 2.0f, 2.0f, 2.0f, 0.0f, 2.0f, 2.0f, 2.0f, 2.0f, 1));

	//remeber bars are in series so bar 0 is the most recently added bar
	EXPECT_NEAR(pd[0]->bidLow, 0.0f, 0.01);
	EXPECT_NEAR(pd[1]->bidLow, 1.0f, 0.01);
}