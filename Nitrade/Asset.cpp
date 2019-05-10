#include "Asset.h"
#include <vector>
#include <map>


Nitrade::Asset::Asset(std::string assetName) : _name(assetName)
{
}

Nitrade::Asset::Asset(std::string assetName, BinaryChunkReader* bReader) :_name(assetName), _bReader(bReader)
{
	_priceData = new std::map<std::string, Nitrade::IPriceData*>();

}

Nitrade::Asset::~Asset()
{
	delete _bReader;
	for (auto a : *_priceData)
		delete (&a)->second;
	delete _priceData;
}

std::string Nitrade::Asset::getName()
{
	return _name;
}

bool Nitrade::Asset::hasBinaryReader()
{
	if (_bReader == nullptr)
		return false;
	return true;
}

bool Nitrade::Asset::openFile()
{
	//throws an exception if it hasn't been assigned
	tryBinaryReader();

	return _bReader->openFile();
}

bool Nitrade::Asset::eof()
{
	//throws an exception if it hasn't been assigned
	tryBinaryReader();

	return _bReader->eof();
}

char* Nitrade::Asset::endChunk()
{
	//throws an exception if it hasn't been assigned
	tryBinaryReader();

	return _bReader->endChunk();
}

char* Nitrade::Asset::getChunk()
{
	//throws an exception if it hasn't been assigned
	tryBinaryReader();

	return _bReader->getChunk();
}

void Nitrade::Asset::closeFile()
{
	//throws an exception if it hasn't been assigned
	tryBinaryReader();

	_bReader->closeFile();
}

void Nitrade::Asset::addPriceData(std::string dataSetName, Nitrade::PriceData* pd)
{
	(*_priceData)[dataSetName] = pd;
}

Nitrade::IPriceData* Nitrade::Asset::getPriceData(std::string dataSetName)
{
	if (_priceData->find(dataSetName) != _priceData->end())
		return (*_priceData)[dataSetName];
	else
		throw std::out_of_range("Dataset " + dataSetName + " does not exist in " + _name);
}

Nitrade::IPriceData** Nitrade::Asset::getAllPriceData()
{
	//use an native array (with max size of 50) because this is much faster to loop through in backtesting
	//50 should be plenty big to hold all the timeframes that might be used
	IPriceData** pdArray = new IPriceData*[50];

	int index = 0;
	for (auto data : *_priceData)
	{
		pdArray[index++] = data.second;
	}

	//add a nullptr on the end so we know this is the end of the array
	pdArray[index] = nullptr;

	return pdArray;
}

void Nitrade::Asset::tryBinaryReader()
{
	if (_bReader == NULL)
		throw std::exception("Null pointer: Can't open file because a BinaryChunkReader has not been assigned for this controller.");
}
