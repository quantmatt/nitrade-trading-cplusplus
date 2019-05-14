#pragma once
#include "IStrategyDefinition.h"
#include "OptimiseParameter.h"
#include <tuple>

namespace Nitrade{
	class StrategyDefinition :
		public IStrategyDefinition
	{
	private:
		std::unique_ptr<std::tuple<std::string, int, int>[]> _dataSetParameters;
		std::unique_ptr<OptimiseParameter[]> _optimiseParameters;
		std::unique_ptr<Strategy> _strategy;

		int _dataSetCount{ 0 };
		int _optimiseParameterCount{ 0 };
	public:
		StrategyDefinition();
		virtual ~StrategyDefinition() = default;

		void setStrategy(std::unique_ptr<Strategy> strategy);
		Strategy* getStrategy();

		std::unique_ptr<std::string[]> getAssetNames() { return nullptr; };

		const std::tuple<std::string, int, int>& getDataSetParams(int index);
		int getDataSetCount();

		const OptimiseParameter& getOptimiseParams(int index);
		int getOptimiseParameterCount();

	};
}

