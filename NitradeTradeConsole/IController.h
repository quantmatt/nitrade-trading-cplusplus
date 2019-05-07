#pragma once

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
	protected:
		BinaryChunkReader* _bReader;
		Strategy* _strategies;
		TradeManager* _tradeManager;
		DataManager* _dataManager;

	public:
		IController() {}
		virtual ~IController() {}

		//binary reader functions
		virtual bool openFile() = 0; //returns the size in bytes of the file
		virtual bool eof() = 0; //returns true if end of file
		virtual char* endChunk() = 0; //pointer to last byte of the chunk
		virtual char* getChunk() = 0;
		virtual void closeFile() = 0; //close the file

		//price data functions
		virtual std::vector<Nitrade::PriceData>* getAssetData(std::string assetName) = 0;

	};
}

