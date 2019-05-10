#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "PriceData.h"
#include "BinaryChunkReader.h"

namespace Nitrade {

	class IAsset
	{
	public:
		IAsset() {}
		IAsset(std::string assetName) {}
		virtual ~IAsset() {}

		//properties
		virtual std::string getName() = 0;

		//binary reader functions
		virtual bool hasBinaryReader() = 0;
		virtual bool openFile() = 0;
		virtual bool eof() = 0;
		virtual char* endChunk() = 0;
		virtual char* getChunk() = 0;
		virtual void closeFile() = 0;

		//price data functions
		virtual void addPriceData(std::string dataSetName, Nitrade::PriceData* pd) = 0;
		virtual Nitrade::IPriceData** getAllPriceData() = 0;


	};

	class Asset 
		: public IAsset
	{
	private:
		float _pipValue{ 0 };
		int _digits{ 0 };
		std::string _name{ "" };

		std::map<std::string, IPriceData*>* _priceData;

		//used for reading data for backtests
		BinaryChunkReader* _bReader{ nullptr };

	public:
		Asset(std::string assetName);
		//create an asset to read data from file
		Asset(std::string assetName, BinaryChunkReader* bReader);
		virtual ~Asset();

		//properties
		std::string getName();

		//binary reader functions
		bool hasBinaryReader();
		bool openFile(); //returns the size in bytes of the file
		bool eof(); //returns true if end of file
		char* endChunk(); //pointer to last byte of the chunk
		char* getChunk();
		void closeFile(); //close the file

		//price data functions
		void addPriceData(std::string dataSetName, Nitrade::PriceData* pd);
		Nitrade::IPriceData** getAllPriceData();

	private:
		void tryBinaryReader();
	};

}

