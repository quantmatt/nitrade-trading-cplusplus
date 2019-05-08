#pragma once
#include <vector>
#include "Structs.h"

namespace Nitrade {


	//Trade Manager will have sub classes different for back testing and live trading
	class TradeManager
	{
	protected:
		std::vector<Trade> trades{};

	public:
		TradeManager() {}
		virtual ~TradeManager() {}

	};
}
