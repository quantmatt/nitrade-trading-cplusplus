#pragma once
#include <iostream>

namespace Utils {

	class StringUtils
	{
	public:
		
		static void strcpy(char* charPointer, std::string stringToCopy, unsigned int size) {
						
			for (unsigned int i = 0; i < size; i++) {
				if (i < stringToCopy.size())
				{
					*charPointer = stringToCopy[i];
				}
				else
					*charPointer = ' ';

				charPointer++;
			}			
		}


	};
}
