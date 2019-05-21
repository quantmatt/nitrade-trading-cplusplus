#pragma once

#include <map>
#include <iostream>

namespace Nitrade {
	class TradeDataManager
	{

	private:
		//trade data is mapped by tradeId, and map of fieldNames and double values
		std::map<int, std::map<std::string, double>> _tradeData;
		//use a map for field names instead of a vector so for every trade we just set _filedNames[value] = true  instead of searching if exists
		std::map<std::string, bool> _fieldNames;

	public:
		TradeDataManager() = default;
		virtual ~TradeDataManager() = default;

		void push_back(int tradeId, std::map<std::string, double>& data);
		bool toBinary(std::string filepath);


	};
}
