#pragma once
#include <memory>
#include <iostream>

namespace Nitrade {
	class IStrategyDefinition
	{
	public:
		IStrategyDefinition() = default;
		virtual ~IStrategyDefinition() = default;

		virtual std::unique_ptr<std::string[]> getAssetNames() = 0;
		virtual const std::tuple<std::string, int, int>& getDataSetParams(int index) = 0;
		virtual int getDataSetCount() = 0;
	};
}

