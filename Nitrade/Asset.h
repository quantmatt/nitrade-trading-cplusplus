#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include "PriceData.h"
#include "BinaryChunkReader.h"
#include "FastAccessDynamicArray.h"

namespace Nitrade {

	class IAsset
	{
	public:
		IAsset() {}
		IAsset(std::string assetName) {}
		virtual ~IAsset() {}

		//properties
		virtual std::string getName() = 0;
		virtual std::string getDataPath() = 0;


	};

	class Asset 
		: public IAsset
	{
	private:
		float _pipValue{ 0 };
		int _digits{ 0 };
		std::string _name{ nullptr };
		std::string _dataPath{  "D:\\TickData\\EURUSD_m1.bin" }; //tempory hard coded

		std::map<std::string, IPriceData*>* _priceData;

		//used for reading data for backtests
		std::unique_ptr<BinaryChunkReader> _bReader;

	public:
		Asset() = default;
		Asset(std::string assetName);
		//create an asset to read data from file
		Asset(std::string assetName, std::unique_ptr<BinaryChunkReader> bReader);
		virtual ~Asset();

		//properties
		std::string getName();
		std::string getDataPath();

	};

}

