#include "pch.h"

#include "../NitradeTradeConsole/BackTest.h"

class BacktestTest : public ::testing::Test
{
protected:
	Nitrade::BackTest* bt;

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};
TEST_F(BacktestTest, CanCreateInstance) {

}