#include "Strategy.h"


Nitrade::Strategy::~Strategy()
{
	//dont delete trade manager or data manager because it is used by all strategies
}

void Nitrade::Strategy::init(ITradeManager* tradeManager, IDataManager* dataManager)
{
	//need a reference to the controllers trade and data managers
	_tradeManager = tradeManager;
	_dataManager = dataManager;	
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
	return (*_currentData)[0]->bidOpen;
}

float Nitrade::Strategy::bidClose(int offset)
{
	return (*_currentData)[0]->bidClose;
}

float Nitrade::Strategy::bidHigh(int offset)
{
	return (*_currentData)[0]->bidHigh;
}

float Nitrade::Strategy::bidLow(int offset)
{
	return (*_currentData)[0]->bidLow;
}

float Nitrade::Strategy::askOpen(int offset)
{
	return (*_currentData)[0]->askOpen;
}

float Nitrade::Strategy::askClose(int offset)
{
	return (*_currentData)[0]->askClose;
}

float Nitrade::Strategy::askHigh(int offset)
{
	return(*_currentData)[0]->askHigh;
}

float Nitrade::Strategy::askLow(int offset)
{
	return (*_currentData)[0]->askLow;
}

int Nitrade::Strategy::volume(int offset)
{
	return (*_currentData)[0]->volume;
}

bool Nitrade::Strategy::setCurrentDataIfRequired(IPriceData* pd)
{
	int size = _requiredData.size();
	for (int i = 0; i < size; i++)
	{
		if (_requiredData[i] == pd)
		{
			_currentData = pd;
			return true;
		}
	}

	return false;
}
