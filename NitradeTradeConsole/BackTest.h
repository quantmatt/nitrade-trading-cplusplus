#pragma once

#include "IController.h"
#include "Structs.h"

namespace Nitrade {


	class BackTest
	{

	public:
		BackTest();
		virtual ~BackTest();

		void Run(IController* controller, std::string assetName); 

	};
}

