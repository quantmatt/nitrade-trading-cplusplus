#pragma once

#include "IController.h"
#include "Structs.h"

namespace Nitrade {


	class BackTest
	{
	private:

		IBinaryChunkReader* _bReader;

	public:
		BackTest();
		virtual ~BackTest();

		void Run(IController* controller, std::string assetName); 

	};
}

