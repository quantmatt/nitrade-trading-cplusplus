#pragma once

#include <fstream>
#include <iostream>

namespace Nitrade {
	class IBinaryChunkReader
	{
	
	public:
		IBinaryChunkReader() {}
		IBinaryChunkReader(std::string filepath) {}
		virtual ~IBinaryChunkReader() {}

		virtual bool isOpen() = 0; //returns true if the file is currently open
		virtual bool openFile() = 0; //returns the size in bytes of the file
		virtual bool eof() = 0; //returns true if end of file
		virtual char* endChunk() = 0; //pointer to last byte of the chunk
		virtual char* getChunk() = 0;
		virtual void closeFile() = 0; //close the file

	};
}

