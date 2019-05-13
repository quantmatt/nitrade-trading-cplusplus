#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Asset.h"

namespace Nitrade {

	class IDataManager
	{
	public:
		IDataManager() = default;
		virtual ~IDataManager() = default;

		virtual void addAsset(std::unique_ptr<IAsset> asset) = 0;
		virtual Nitrade::IAsset* getAsset(std::string assetName) = 0;
		virtual std::vector<std::string>* getAssetNames() = 0;

	};

	class DataManager :
		public IDataManager
	{
		
	private:
		std::map<std::string, std::unique_ptr<IAsset>>* _assets{ nullptr };

	public:
		DataManager();
		virtual ~DataManager();

		void addAsset(std::unique_ptr<IAsset> asset);
		Nitrade::IAsset* getAsset(std::string assetName);
		std::vector<std::string>* getAssetNames();
	};
}

