#pragma once
#include <vector>
#include "Structs.h"

namespace Nitrade {

	class ITradeManager
	{
	public:
		ITradeManager();
		virtual ~ITradeManager();

	};


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
