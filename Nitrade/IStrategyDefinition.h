#pragma once
#include <memory>
#include <iostream>
#include "Structs.h"
#include "Strategy.h"
#include "OptimiseParameter.h"

namespace Nitrade {
	class IStrategyDefinition
	{
	public:
		IStrategyDefinition() = default;
		virtual ~IStrategyDefinition() = default;

		virtual std::unique_ptr<std::string[]> getAssetNames() = 0;

		virtual void setStrategy(std::unique_ptr<Strategy> strategy) = 0;
		virtual Strategy* getStrategy() = 0;

		virtual const std::tuple<std::string, int, int>& getDataSetParams(int index) = 0;
		virtual int getDataSetCount() = 0;

		virtual const OptimiseParameter& getOptimiseParams(int index) = 0;
		virtual int getOptimiseParameterCount() = 0;
	};
}

