

#include "Controller.h"


Nitrade::Controller::Controller()
{
	//assign pointers to null so we can check if they have been assigned later
	_bReader = NULL;
	_strategies = NULL;
	_tradeManager = NULL;
	_dataManager = NULL;
}


Nitrade::Controller::~Controller()
{

}

void Nitrade::Controller::setBinaryReader(BinaryChunkReader* bReader)
{
	if (_bReader != NULL)
		delete _bReader;
	_bReader = bReader;
}

bool Nitrade::Controller::hasBinaryReader()
{
	if (_bReader == NULL)
		return false;
	return true;
}

bool Nitrade::Controller::openFile()
{
	//throws an exception if it hasn't been assigned
	tryBinaryReader();

	return _bReader->openFile();
}

bool Nitrade::Controller::eof()
{
	//throws an exception if it hasn't been assigned
	tryBinaryReader();

	return _bReader->eof();
}

char* Nitrade::Controller::endChunk()
{
	//throws an exception if it hasn't been assigned
	tryBinaryReader();

	return _bReader->endChunk();
}

char* Nitrade::Controller::getChunk()
{
	//throws an exception if it hasn't been assigned
	tryBinaryReader();

	return _bReader->getChunk();
}

void Nitrade::Controller::closeFile()
{
	//throws an exception if it hasn't been assigned
	tryBinaryReader();

	_bReader->closeFile();

}

std::vector<Nitrade::IPriceData*>* Nitrade::Controller::getAssetData(std::string assetName)
{
	//TODO actual implementation
	PriceData* pd = new PriceData(200,60);

	std::vector<Nitrade::IPriceData*>* priceData = new std::vector<Nitrade::IPriceData*>();
	priceData->push_back(pd);

	return priceData;
}

void Nitrade::Controller::onBar()
{
	//TODO
}

void Nitrade::Controller::tryBinaryReader()
{
	if (_bReader == NULL)
		throw std::exception("Null pointer: Can't open file because a BinaryChunkReader has not been assigned for this controller.");
}
