#pragma once
#include "IStrategyDefinition.h"
#include "IDataFactory.h"
#include <memory>
#include <iostream>

namespace Nitrade {
	class HistoricSimulator
	{
	private:

		std::unique_ptr<IStrategyDefinition> _strategyDefinition;
		std::unique_ptr<IDataFactory> _dataFactory;
		std::vector<std::unique_ptr<Asset>> _loadedAssets;

	public:
		HistoricSimulator() = default;		
		virtual ~HistoricSimulator() = default;

		void Setup(std::unique_ptr<IStrategyDefinition> strategyDef,
			std::unique_ptr<IDataFactory> dataFactory = nullptr);

		void Optimise(int threads);
		void Optimise(std::string assetName);

	private:
		bool isBarValid(const Nitrade::Bar* bar);
		Asset& getAsset(std::string assetName);
	
	};
}

