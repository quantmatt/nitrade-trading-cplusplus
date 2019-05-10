#pragma once

#include <iostream>
#include <vector>
#include "BinaryChunkReader.h"
#include "Strategy.h"
#include "TradeManager.h"
#include "DataManager.h"
#include "PriceData.h"

namespace Nitrade {

	//Controller is used for both back testing and live trading - although different sub classes will be used for TradeManager
	class IController
	{
	

	public:
		IController() {}
		virtual ~IController() {}

		//binary reader functions
		virtual bool hasBinaryReader(std::string assetName) = 0;
		virtual bool openFile(std::string assetName) = 0; //returns the size in bytes of the file
		virtual bool eof(std::string assetName) = 0; //returns true if end of file
		virtual char* endChunk(std::string assetName) = 0; //pointer to last byte of the chunk
		virtual char* getChunk(std::string assetName) = 0;
		virtual void closeFile(std::string assetName) = 0; //close the file

		virtual void addAsset(IAsset* asset) = 0;
		virtual IAsset* getAsset(std::string assetName) = 0;
		virtual std::vector<std::string>* getAssetNames() = 0;
		

		//strategy functions
		virtual void run(IPriceData* pd) = 0;

	};
}

