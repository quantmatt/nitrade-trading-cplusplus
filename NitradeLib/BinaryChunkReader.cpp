#include "BinaryChunkReader.h"


using namespace std;

Nitrade::BinaryChunkReader::BinaryChunkReader(std::string filepath)
{
	//default the chunk size to 50 MB
	_chunkSize = 50;

	//set the filepath this binarychunkreader will operate on
	_filepath = filepath;

	//convert megabytes to bytes
	_bufferSize = _chunkSize * 1024 * 1024;

	//round the buffer size to whole Bars struct
	_bufferSize = (_bufferSize / sizeof(Bar)) * sizeof(Bar);

}


Nitrade::BinaryChunkReader::~BinaryChunkReader()
{
	if (_inputFile.is_open())
		_inputFile.close();
}

bool Nitrade::BinaryChunkReader::isOpen()
{
	if (_inputFile.is_open())
		return true;
	

	return false;
}

bool Nitrade::BinaryChunkReader::openFile()
{

	if (_filepath == "")
		throw invalid_argument("Filepath in binaryChunkReader has not been supplied.");

	//reading from D drive seems to be about 10% faster again - maybe because program is running on D drive
	_inputFile.open(_filepath, std::ios::binary | std::ios::ate);

	if (_inputFile.is_open())
	{
		//get the size of the file and move to start of file
		const std::streamsize size = _inputFile.tellg();
		_size = size;
		_inputFile.seekg(0, std::ios::beg);

		//don't allow the buffer to be larger than the actual file size
		if (_bufferSize > size)
			_bufferSize = (int)size;

		return true;
	}

	return false;
}

bool Nitrade::BinaryChunkReader::eof()
{
	return _processedBytes == _size;
}

char* Nitrade::BinaryChunkReader::endChunk()
{
	return	_lastChunkByte;	
}

char* Nitrade::BinaryChunkReader::getChunk()
{
	//if the file isn't open jsut return a null pointer
	//and set the last chunk byte to a nullptr
	if (!_inputFile.is_open())
	{
		_lastChunkByte = nullptr;
		return nullptr;
	}


	//declare the binaryData array to hold the whole contents of the file.
	_pBinData = std::make_unique<char[]>(_bufferSize);

	//make sure we don't read more than what is left
	int remaining = (int)(_size - _processedBytes);
	if (remaining < _bufferSize)
		_bufferSize = remaining;

	//read the file into the char array
	if (_inputFile.read(_pBinData.get(), _bufferSize))
	{
		_processedBytes += _bufferSize;

		_lastChunkByte = _pBinData.get() + _bufferSize;

		//return a raw pointer to the char array owned by BinaryChunkReader
		return _pBinData.get();
	}

	return nullptr;
}

void Nitrade::BinaryChunkReader::closeFile()
{
	_size = 0;
	_processedBytes = 0;
	_inputFile.close();
}
