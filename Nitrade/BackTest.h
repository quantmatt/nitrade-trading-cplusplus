#pragma once

#include "IController.h"
#include "Structs.h"

namespace Nitrade {


	class BackTest
	{

	public:
		BackTest();
		virtual ~BackTest();

		void RunAll(IController* controller);
		void Run(IController* controller, std::string assetName); 
	
	private:
		bool isBarValid(const Nitrade::Bar* bar); //checks to see if the bar can sensical values eg. if loaded from a binary file with wrong format

	};
}

