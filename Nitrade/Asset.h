#pragma once
#include <iostream>
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

		

	public:
		Asset() = default;
		Asset(std::string assetName);
		virtual ~Asset() = default;

		//properties
		std::string getName();
		std::string getDataPath();

	};

}

