#include "Asset.h"
#include "FastAccessDynamicArray.h"
#include <vector>
#include <map>


Nitrade::Asset::Asset(std::string assetName) : _name(assetName)
{
}

Nitrade::Asset::Asset(std::string assetName, std::unique_ptr<BinaryChunkReader> bReader) :_name(assetName)
{
	_bReader = std::move(bReader);
	_priceData = new std::map<std::string, Nitrade::IPriceData*>();

}

Nitrade::Asset::~Asset()
{
	/*
	for (auto a : *_priceData)
		delete (&a)->second;
	delete _priceData;
	*/
}

std::string Nitrade::Asset::getName()
{
	return _name;
}

std::string Nitrade::Asset::getDataPath()
{
	return _dataPath;
}
