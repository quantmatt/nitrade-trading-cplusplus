#include "Strategy.h"

Nitrade::Strategy::~Strategy()
{
	//dont delete trade manager or data manager because it is used by all strategies
}

std::unique_ptr<Nitrade::Strategy> Nitrade::Strategy::clone()
{
	return std::make_unique<Strategy>();
}

void Nitrade::Strategy::init(ITradeManager* tradeManager, IAssetData* assetData)
{
	//need a reference to the controllers trade and data managers
	_tradeManager = tradeManager;
	_assetData = assetData;
	
	//initialse any strategy specific code
	onInit();
}

void Nitrade::Strategy::setParameter(std::string name, double value)
{
	_parameters[name] = value;
}

double Nitrade::Strategy::getParameter(std::string name)
{
	return _parameters[name];
}

bool Nitrade::Strategy::openTrade(std::string asset, tradeDirection direction, int size, double stopLoss, double takeProfit)
{
	return false;
}

bool Nitrade::Strategy::closeTrade(long tradeId)
{
	return false;
}

int Nitrade::Strategy::getOpenTradeCount(std::string assetName, std::string strategyName)
{
	return 0;
}

int Nitrade::Strategy::getOpenTradeCount()
{
	return 0;
}

double Nitrade::Strategy::getPip()
{
	//gets the value of 1 pip for the currently selected asset
	return 0.0001;
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
	int size = _assetData->size();
	for (int i = 0; i < size; i++)
	{
		if ((*_assetData)[i] == pd)
		{
			_currentData = pd;
			return true;
		}
	}

	return false;
}
