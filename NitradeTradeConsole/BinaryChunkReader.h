#pragma once
#include "IBinaryChunkReader.h"

namespace Nitrade {
	class BinaryChunkReader :
		public IBinaryChunkReader
	{
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

