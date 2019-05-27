#include "gtest/gtest.h"
#include "NitradeLib.h"

#include "MockAssetData.cc"
#include "MockPriceData.cc"
#include "MockTradeManager.cc"

using namespace std;
using namespace Utils;

class StrategySetTest : public ::testing::Test
{
protected:

	unique_ptr<MockAssetData> mockAssetData = make_unique<MockAssetData>();
	unique_ptr<MockTradeManager> mockTradeManager = make_unique<MockTradeManager>();
	unique_ptr<MockPriceData> mockPriceData = make_unique<MockPriceData>();
	StrategySet ss;

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