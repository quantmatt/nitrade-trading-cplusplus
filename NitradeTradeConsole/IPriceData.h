#pragma once

#include "Structs.h"

namespace Nitrade {
	class IPriceData
	{

	public:
		IPriceData() {}
		virtual ~IPriceData() {}

		virtual void init(int lookback, int barSize, int arraySizeFactor = 100) = 0;
		virtual bool updateCurrentBarFromBar(Nitrade::Bar* newInfo) = 0;
		virtual bool updateCurrentBarFromTick(float quote, bool isBid) = 0;

	};
}

