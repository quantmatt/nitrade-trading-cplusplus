#pragma once
#include "IStrategyDefinition.h"
#include <tuple>

namespace Nitrade{
	class StrategyDefinition :
		public IStrategyDefinition
	{
	private:
		std::unique_ptr<std::tuple<std::string, int, int>[]> _dataSetParameters;
		int _dataSetCount = 1;
	public:
		StrategyDefinition();
		virtual ~StrategyDefinition() = default;

		std::unique_ptr<std::string[]> getAssetNames() { return nullptr; };
		const std::tuple<std::string, int, int>& getDataSetParams(int index);
		int getDataSetCount();

	};
}

