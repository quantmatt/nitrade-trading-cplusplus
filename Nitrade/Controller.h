#pragma once
#include "IController.h"
#include "PriceData.h"
#include <vector>

namespace Nitrade {
	class Controller :
		public IController
	{

	private:
		
		Strategy* _strategies{ nullptr };
		TradeManager* _tradeManager{ nullptr };
		DataManager* _dataManager{ nullptr };

	public:
		Controller();
		virtual ~Controller();

		//binary reader functions
		bool hasBinaryReader(std::string assetName);
		bool openFile(std::string assetName); //returns the size in bytes of the file
		bool eof(std::string assetName); //returns true if end of file
		char* endChunk(std::string assetName); //pointer to last byte of the chunk
		char* getChunk(std::string assetName);
		void closeFile(std::string assetName); //close the file

		

		//asset functions
		void addAsset(IAsset* asset);
		IAsset* getAsset(std::string assetName);
		std::vector<std::string>* getAssetNames();

		//strategy functions
		void onBar(std::string assetName);

	private:
		//void tryBinaryReader();
	};
}

