#pragma once
#include <iostream>
namespace Nitrade {

	class IAsset
	{
	public:
		IAsset() {}
		IAsset(std::string csv) {}
		virtual ~IAsset() {}

		//properties
		virtual std::string getName() = 0;
		virtual std::string getDataPath() = 0;


	};

	class Asset 
		: public IAsset
	{
	private:
		float _pip{ 0 };
		float _pipValue{ 0 };
		int _digits{ 0 };
		float _pipCost{ 0 };
		std::string _name{ (10, ' ') };
		std::string _dataPath{""};

		

	public:
		Asset() = default;
		Asset(std::string csv);
		virtual ~Asset() = default;

		//properties
		std::string getName();
		std::string getDataPath();

	};

}

