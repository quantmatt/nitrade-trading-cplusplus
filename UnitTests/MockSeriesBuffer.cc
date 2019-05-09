#pragma once
#include "gmock/gmock.h"  // Brings in Google Mock. - installed from nuget
#include "NitradeLib.h"


using namespace Utils;
using namespace std;

template <class T>
class MockSeriesBuffer : public ISeriesBuffer<T> {
public:

	MOCK_METHOD1_T(add, void(T value));
	MOCK_METHOD1_T(get, T& (int pos));
};