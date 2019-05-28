#include "gtest/gtest.h"

#include "MockAssetData.cc"
#include "MockPriceData.cc"
#include "MockAsset.cc"
#include "MockTradeManager.cc"
#include "NitradeLib.h"
#include "TestStrategy.h"

using namespace std;
using namespace Utils;

class StrategySetTest : public ::testing::Test
{
protected:


	unique_ptr<MockAsset> mockAsset = make_unique<MockAsset>();
	unique_ptr<MockAssetData> mockAssetData = make_unique<MockAssetData>();
	unique_ptr<MockTradeManager> mockTradeManager = make_unique<MockTradeManager>();
	unique_ptr<MockPriceData> mockPriceData = make_unique<MockPriceData>();
	StrategySet ss;
	TestStrategy ts;

};

TEST_F(StrategySetTest, InitBeforeCreateDoesNothing) {	
	ss.init(mockTradeManager.get(), mockAssetData.get());
}

TEST_F(StrategySetTest, RunBeforeCreateDoesNothing) {
	ss.run(mockPriceData.get());
}

TEST_F(StrategySetTest, GetStrategyKeysBeforeCreateReturnsZeroVector) {
	auto results = ss.getStrategyKeys();
	EXPECT_EQ(results.size(), 0);
}

TEST_F(StrategySetTest, createStrategySetTestStrategyParameters) {
	

	ss.createFrom(&ts, mockAsset.get());
	auto keys = ss.getStrategyKeys();

	//check the right number of strategy variants were created
	ASSERT_EQ(keys.size(), 9);

	//check the strategy parameters were setup correctly
	int strategyIndex = 0;
	for (int i = 5; i < 8; i++)
	{
		for (int z = 20; z < 23; z++)
		{
			ASSERT_EQ((int)ss[strategyIndex]->getParameter("Period1"), i);
			ASSERT_EQ((int)ss[strategyIndex]->getParameter("Period2"), z);
			strategyIndex++;
		}
	}

	
}

TEST_F(StrategySetTest, createStrategySetNoParameters) {

	Strategy empty;
	ss.createFrom(&empty, mockAsset.get());
	auto keys = ss.getStrategyKeys();

	ASSERT_EQ(keys.size(), 1);
}