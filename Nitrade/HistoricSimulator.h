#pragma once
#include "IDataFactory.h"
#include "Strategy.h"
#include "DataFactory.h"
#include <memory>
#include <iostream>

namespace Nitrade {
	class HistoricSimulator
	{
	private:

		std::unique_ptr<Strategy> _strategy;
		std::unique_ptr<IDataFactory> _dataFactory;
		std::vector<std::unique_ptr<ITradeManager>> _threadTradeManagers;
		

	public:
		HistoricSimulator() = default;		
		virtual ~HistoricSimulator() = default;

		void setup(std::unique_ptr<Strategy> strategy,
			std::unique_ptr<IDataFactory> dataFactory = nullptr);

		void optimise(int  cpus, std::vector<std::string> assets, bool runningPL=true);
		void optimise(std::string assetName, bool runningPL=true);

	protected:
		void optimiseAssets(ITradeManager* tradeManager, std::vector<std::string> assets, bool runningPL);

	private:
		bool isBarValid(const Nitrade::Bar* bar);
		
	
	
	};
}

