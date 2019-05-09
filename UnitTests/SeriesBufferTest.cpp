#include "gtest/gtest.h"
#include "NitradeLib.h"
using namespace std;
using namespace Utils;

class SeriesBufferTest : public ::testing::Test
{
protected:
	
	void fill(int count, SeriesBuffer<int> &sb)
	{
		sb.add(1);
		sb.add(2);
		sb.add(3);
		sb.add(4);
		sb.add(5);
	}


};

TEST_F(SeriesBufferTest, TestAdd) {

	SeriesBuffer<int> sb(3);
	
	//overfille the buffer and see if the series is returned correctly
	fill(5, sb);

	EXPECT_EQ(sb.get(0), 5);
	EXPECT_EQ(sb.get(1), 4);
	EXPECT_EQ(sb.get(2), 3);
}

TEST_F(SeriesBufferTest, TestIndexOutOfRange) {

	SeriesBuffer<int> sb(3);

	//overfille the buffer but request an index outside the size of the buffer
	fill(5, sb);

	EXPECT_THROW(sb.get(3), out_of_range); //should only have 0,1,2 indexes
	EXPECT_THROW(sb.get(20), out_of_range);
}

TEST_F(SeriesBufferTest, TestIndexNegative) {

	SeriesBuffer<int> sb(1);
	
	EXPECT_THROW(sb.get(-20), out_of_range);
}

TEST_F(SeriesBufferTest, TestIndexNotYetAddedForIntReturnsZero) {

	SeriesBuffer<int> sb(30);

	//partially fill a buffer and request an index in range but hasn't been assigned yet
	fill(2, sb);

	EXPECT_EQ(sb.get(10), 0);	
}


TEST_F(SeriesBufferTest, TestIndexNotYetAddedForPointerReturnsNull) {

	SeriesBuffer<char*> sb(30);

	char c = 'c';
	char* a = &c;
	//test the iterator on a partially filled buffer - should throw exception
	sb.add(a);

	EXPECT_EQ(sb.get(10), nullptr);
}


TEST_F(SeriesBufferTest, TestIteratorPartialFilled) {

	SeriesBuffer<int> sb(30);

	//test the iterator on a partially filled buffer - add 5,4,3,2,1 and the rest as zeros
	fill(5, sb);

	int tally = 0;
	for (int value : sb)
	{
		tally += value;
	}

	EXPECT_EQ(tally, 15);
	
}

TEST_F(SeriesBufferTest, TestIteratorOverFilled) {

	SeriesBuffer<int> sb(3);

	//overfill buffer and test iterator returns the the correct values
	fill(5, sb);

	int i = 5;
	for (int value : sb)
	{
		EXPECT_EQ(value, i--);
	}

	//should not go to zero because buffer is only size 3
	EXPECT_EQ(i, 2);
}

TEST_F(SeriesBufferTest, TestIteratorFilled) {

	SeriesBuffer<int> sb(5);

	//Exactly fill a buffer and test iterator returns correct values
	fill(5, sb);

	int i = 5;
	for (int value : sb)
	{
		EXPECT_EQ(value, i--);
	}

	//should go to exactly zero
	EXPECT_EQ(i, 0);
}