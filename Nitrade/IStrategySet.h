#pragma once

#include "PriceData.h"

namespace Nitrade {
	class IStrategySet
	{
	public:
		IStrategySet() = default;
		virtual ~IStrategySet() = default;
		
		virtual void run(const IPriceData* dataSet) = 0;
	};
}

