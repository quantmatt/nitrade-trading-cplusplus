#include <iostream>
#include <map>
#include "DataManager.h"


Nitrade::DataManager::DataManager()
{
	_assets = new std::map<std::string, Nitrade::IAsset*>();
}


Nitrade::DataManager::~DataManager()
{
	for (auto a : *_assets)
		delete (&a)->second;	
	delete _assets;
}



void Nitrade::DataManager::addAsset(IAsset* asset)
{
	std::string name = asset->getName();
	//store the name of the asset as the map key for easy referencing
	(*_assets)[name] = asset;
}

Nitrade::IAsset* Nitrade::DataManager::getAsset(std::string assetName)
{
	if (_assets->find(assetName) != _assets->end())
		return (*_assets)[assetName];

	throw std::out_of_range(assetName + " is not in the asset list.");
}

std::vector<std::string>* Nitrade::DataManager::getAssetNames()
{
	std::vector<std::string>* names = new std::vector<std::string>();

	for (auto asset : *_assets)
		names->push_back(asset.first);

	return names;
}

