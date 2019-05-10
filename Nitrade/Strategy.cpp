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

void Nitrade::Strategy::addRequireData(std::string assetName, std::string datasetName)
{
	if (_dataManager == nullptr)
		throw std::exception("DataManager must be set before adding required data pointers.");

	//try to find the pointers to this data - will throw exception if asset or dataset doesn't exist
	Nitrade::IPriceData* pd = _dataManager->getAsset(assetName)->getPriceData(datasetName);


	//create a temp array one size larger
	IPriceData** temp = new IPriceData * [_requiredDataCount + 1];

	//copy the pointers across
	for (int i = 0; i < _requiredDataCount; i++)
		temp[i] = _requiredData[i];

	_requiredData = temp;

	//add in the new strategy and increment the counter
	_requiredData[_requiredDataCount] = pd;
	_requiredDataCount++;

	
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

bool Nitrade::Strategy::setCurrentDataIfRequired(IPriceData* pd)
{
	for (int i = 0; i < _requiredDataCount; i++)
	{
		if (_requiredData[i] == pd)
		{
			_currentData = pd;
			return true;
		}
	}

	return false;
}
