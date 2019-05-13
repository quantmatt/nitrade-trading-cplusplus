#pragma once

#include "PriceData.h"

namespace Nitrade {
	class IAssetData
	{
	public:
		IAssetData() = default;
		virtual ~IAssetData() = default;

		virtual int size() = 0;
		virtual IPriceData* operator [](int pos) = 0;
	};
}
