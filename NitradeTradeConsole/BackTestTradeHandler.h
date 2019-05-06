#pragma once

#include "TradeHandler.h"


namespace Nitrade {
	class BackTestTradeHandler :
		public TradeHandler
	{
	public:
		BackTestTradeHandler();
		~BackTestTradeHandler();
	};
}

