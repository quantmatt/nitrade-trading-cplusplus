#pragma once

#include <vector>
#include "Structs.h"

namespace Nitrade {


	class BackTest
	{
	private:
		std::vector<Trade> trades;

	public:
		BackTest();
		virtual ~BackTest();

		double ChunkArray(int bufferSizeMB);

	};
}

