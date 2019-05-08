#pragma once
#include "IBinaryChunkReader.h"

namespace Nitrade {
	class BinaryChunkReader :
		public IBinaryChunkReader
	{
	private:
		std::ifstream _inputFile{};
		std::string _filepath{};
		char* _lastChunkByte{};
		int _chunkSize{ 0 }; //size in MB of chunk of binary data to read at a time
		int _bufferSize{ 0 }; //size of the buffer rounded to the nearest Bar size
		long long _size{ 0 }; //total bytes of loaded file
		long long _processedBytes{ 0 }; //number of bytes processed
		char* _pBinData{};

	public:
		BinaryChunkReader() {}
		BinaryChunkReader(std::string filepath);
		virtual ~BinaryChunkReader();

		bool isOpen();
		bool openFile();
		bool eof();
		char* endChunk();
		char* getChunk();
		void closeFile();

	};
}

