#include "Strategy.h"
#include "TestStrategy.h"


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


const std::tuple<std::string, int, int>& Nitrade::Strategy::getDataSetParams(int index)
{
	return std::move(_dataSetParameters[index]);
}

int Nitrade::Strategy::getDataSetCount()
{
	return _dataSetParameters.size();
}

const Nitrade::OptimiseParameter& Nitrade::Strategy::getOptimiseParams(int index)
{
	return _optimiseParameters[index];
}

int Nitrade::Strategy::getOptimiseParameterCount()
{
	return _optimiseParameters.size();
}


void Nitrade::Strategy::setAsset(Nitrade::IAsset* asset)
{
	_currentAsset = asset; 
}

bool Nitrade::Strategy::openTrade(tradeDirection direction, int size, float stopLoss, float takeProfit)
{
	//this will be the current bar - ie. the one that has not closed yet so careful not to use any close price or volume info
	//the open of the bar is essentially the first oportunity to open a trade based on calculations from the last bar
	Bar* bar = (*_currentData)[0];

	//built in spread filter
	if ((bar->askOpen - bar->bidOpen) / getPip() > _spreadFilter)
		return false;

	auto trade = std::make_unique<Trade>();
	trade->openTime = bar->timestamp;	
	//copy max string length of 10 chars to the structs char array	
	strcpy_s(trade->assetName, getAssetName().c_str());
	trade->variantId = _variantId;
	trade->size = size;
	trade->direction = direction;
	trade->stopLoss = stopLoss;
	trade->takeProfit = takeProfit;
	trade->spread = (float)((bar->askOpen - bar->bidOpen) / getPip());
	if (direction == tradeDirection::Long)
		trade->openLevel = bar->askOpen;
	else
		trade->openLevel = bar->bidOpen;

	_tradeManager->openTrade(std::move(trade), _data);

	return true;
}

bool Nitrade::Strategy::closeTrade(int tradeId)
{
	return false;
}

void Nitrade::Strategy::closeTrades()
{
	_tradeManager->closeTrades(getAssetName(), _variantId, (*_currentData)[0]);
}

void Nitrade::Strategy::closeTrades(tradeDirection direction)
{
	_tradeManager->closeTrades(getAssetName(), _variantId, direction, (*_currentData)[0]);
}

int Nitrade::Strategy::getOpenTradeCount(std::string assetName, std::string strategyName)
{
	return 0;
}

int Nitrade::Strategy::getOpenTradeCount()
{
	return _tradeManager->getOpenTradeCount(getAssetName(), _variantId);
}


float Nitrade::Strategy::bidOpen(unsigned int offset)
{
	return (*_currentData)[offset+1]->bidOpen;
}

float Nitrade::Strategy::bidClose(unsigned int offset)
{
	return (*_currentData)[offset + 1]->bidClose;
}

float Nitrade::Strategy::bidHigh(unsigned int offset)
{
	return (*_currentData)[offset + 1]->bidHigh;
}

float Nitrade::Strategy::bidLow(unsigned int offset)
{
	return (*_currentData)[offset + 1]->bidLow;
}

float Nitrade::Strategy::askOpen(unsigned int offset)
{
	return (*_currentData)[offset + 1]->askOpen;
}

float Nitrade::Strategy::askClose(unsigned int offset)
{
	return (*_currentData)[offset + 1]->askClose;
}

float Nitrade::Strategy::askHigh(unsigned int offset)
{
	return(*_currentData)[offset + 1]->askHigh;
}

float Nitrade::Strategy::askLow(unsigned int offset)
{
	return (*_currentData)[offset + 1]->askLow;
}

int Nitrade::Strategy::volume(unsigned int offset)
{
	return (*_currentData)[offset + 1]->volume;
}

bool Nitrade::Strategy::setCurrentDataIfRequired(IPriceData* pd)
{
	/*
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
	*/
	_currentData = pd;
	if (pd->getBarIndex() > 1)
		return true;
	else
		return false;
}
