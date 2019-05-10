

#include "Controller.h"


Nitrade::Controller::Controller()
{
	_dataManager = new DataManager();
}


Nitrade::Controller::~Controller()
{
	delete _dataManager;
	delete _tradeManager;
	delete _strategies;
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

void Nitrade::Controller::onBar(std::string assetName)
{
	//TODO
}

