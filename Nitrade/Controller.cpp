#include "Controller.h"


Nitrade::Controller::Controller()
{
	_dataManager = new DataManager();
	_tradeManager = new TradeManager();
}


Nitrade::Controller::~Controller()
{
	delete _dataManager;
	delete _tradeManager;
	
}

bool Nitrade::Controller::hasBinaryReader(std::string assetName)
{
	Asset* asset = (Asset*)getAsset(assetName);
	if (asset != nullptr)
		return asset->hasBinaryReader();

	return false;
}

bool Nitrade::Controller::openFile(std::string assetName)
{
	//throws an exception if it hasn't been assigned
	Asset* asset = (Asset*)getAsset(assetName);
	if (asset != nullptr)
		return asset->openFile();

	return false;
}

bool Nitrade::Controller::eof(std::string assetName)
{
	Asset* asset = (Asset*)getAsset(assetName);
	if (asset != nullptr)
		return asset->eof();

	return true;
}

char* Nitrade::Controller::endChunk(std::string assetName)
{
	Asset* asset = (Asset*)getAsset(assetName);
	if (asset != nullptr)
		return asset->endChunk();

	return nullptr;
}

char* Nitrade::Controller::getChunk(std::string assetName)
{	
	Asset* asset = (Asset*)getAsset(assetName);
	if (asset != nullptr)
		return asset->getChunk();

	return nullptr;
}

void Nitrade::Controller::closeFile(std::string assetName)
{
	//throws an exception if it hasn't been assigned
	Asset* asset = (Asset*)getAsset(assetName);
	if (asset != nullptr)
		asset->closeFile();

}

void Nitrade::Controller::addAsset(IAsset* asset)
{
	_dataManager->addAsset(asset);
}

Nitrade::IAsset* Nitrade::Controller::getAsset(std::string assetName)
{
	if (_dataManager != nullptr)
		return _dataManager->getAsset(assetName);
	else
		return nullptr;
}

std::vector<std::string>* Nitrade::Controller::getAssetNames()
{
	if (_dataManager != nullptr)
		return _dataManager->getAssetNames();
	else
		return nullptr;
}

void Nitrade::Controller::run(IPriceData* pd)
{
	//Need to loop through possibly thousands of strategies - so vector is too slow in iteration
	int size = _strategies.size();
	for (int i = 0; i < size; i++)
	{
		Strategy* strategy = _strategies[i];
		if(strategy->setCurrentDataIfRequired(pd))
			strategy->onBar();
	}
}

void Nitrade::Controller::addStrategy(Strategy* strategy)
{	

	//Initialise the strategy with the tradeManager and dataManager pointers
	strategy->init(_tradeManager, _dataManager);


	//Need to loop through possibly thousands of strategies - so vector is too slow in iteration / using fastAccessDynamicArray
	_strategies.add(strategy);

}

