#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "Asset.h"

namespace Nitrade {

	class IDataManager
	{
	public:
		IDataManager() {}
		virtual ~IDataManager() {}

		virtual void addAsset(IAsset* asset) = 0;
		virtual IAsset* getAsset(std::string assetName) = 0;
		virtual std::vector<std::string>* getAssetNames() = 0;

	};

	class DataManager :
		public IDataManager
	{
		
	private:
		std::map<std::string, IAsset*>* _assets{ nullptr };

	public:
		DataManager();
		virtual ~DataManager();

		void addAsset(IAsset* asset);
		IAsset* getAsset(std::string assetName);
		std::vector<std::string>* getAssetNames();
	};
}

