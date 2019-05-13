#pragma once
#include "IStrategySet.h"
#include "PriceData.h"

namespace Nitrade{
	class StrategySet :
		public IStrategySet
	{
	public:
		StrategySet();
		virtual ~StrategySet();

		void run(const IPriceData* dataSet) {}
	};
}

