#include "Asset.h"


Nitrade::Asset::Asset(std::string assetName) : _name(assetName)
{
}

std::string Nitrade::Asset::getName()
{
	return _name;
}

std::string Nitrade::Asset::getDataPath()
{
	return _dataPath;
}
