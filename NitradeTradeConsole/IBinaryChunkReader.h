#pragma once

#include <fstream>
#include <iostream>

namespace Nitrade {
	class IBinaryChunkReader
	{
	protected:
		std::ifstream _inputFile;
		std::string _filepath;
		char* _lastChunkByte;
		int _chunkSize; //size in MB of chunk of binary data to read at a time
		int _bufferSize; //size of the buffer rounded to the nearest Bar size
		long long _size; //total bytes of loaded file
		long long _processedBytes; //number of bytes processed
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

