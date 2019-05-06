#pragma once

#include <iostream>
#include <vector>
#include "Structs.h"

namespace Nitrade {

	class TradeHandler
	{
	public:
		TradeHandler();
		virtual ~TradeHandler();

		int OpenTradeCount(std::string ref);

	private:
		std::vector<Trade> trades;
	};

}
