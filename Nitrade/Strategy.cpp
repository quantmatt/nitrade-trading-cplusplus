#include "Strategy.h"


Nitrade::Strategy::~Strategy()
{
	//dont delete trade manager or data manager because it is used by all strategies
}

bool Nitrade::Strategy::openTrade(std::string asset, int size, double stopLoss, double takeProfit)
{
	return false;
}

bool Nitrade::Strategy::closeTrade(long tradeId)
{
	return false;
}

float Nitrade::Strategy::bidOpen(int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidClose(int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidHigh(int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidLow(int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidOpen(std::string asset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidClose(std::string asset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidHigh(std::string asset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidLow(std::string asset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidOpen(std::string asset, std::string dataset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidClose(std::string asset, std::string dataset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidHigh(std::string asset, std::string dataset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::bidLow(std::string asset, std::string dataset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askOpen(int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askClose(int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askHigh(int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askLow(int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askOpen(std::string asset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askClose(std::string asset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askHigh(std::string asset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askLow(std::string asset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askOpen(std::string asset, std::string dataset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askClose(std::string asset, std::string dataset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askHigh(std::string asset, std::string dataset, int offset)
{
	return 0.0f;
}

float Nitrade::Strategy::askLow(std::string asset, std::string dataset, int offset)
{
	return 0.0f;
}

int Nitrade::Strategy::volume(int offset)
{
	return 0;
}

int Nitrade::Strategy::volume(std::string asset, int offset)
{
	return 0;
}

int Nitrade::Strategy::volume(std::string asset, std::string dataset, int offset)
{
	return 0;
}
