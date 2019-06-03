#include "AssetData.h"
#include <memory>

Nitrade::AssetData::AssetData(int size)
{
	_priceData = std::make_unique<std::unique_ptr<IPriceData>[]>(size);
	_size = size;
}

void Nitrade::AssetData::set(int index, std::string name, int lookback, int barSize)
{
	_priceData[index] = std::make_unique<PriceData>(lookback, barSize);
}
