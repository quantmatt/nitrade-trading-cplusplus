#pragma once
#include <fstream>
#include <iostream>


namespace Nitrade {

	//interface for mocking
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

	class BinaryChunkReader :
		public IBinaryChunkReader
	{
	private:
		std::ifstream _inputFile{};
		std::string _filepath{};
		char* _lastChunkByte{nullptr};
		int _chunkSize{ 0 }; //size in MB of chunk of binary data to read at a time
		int _bufferSize{ 0 }; //size of the buffer rounded to the nearest Bar size
		long long _size{ 0 }; //total bytes of loaded file
		long long _processedBytes{ 0 }; //number of bytes processed
		char* _pBinData{nullptr};

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

