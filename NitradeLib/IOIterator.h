#pragma once

#include <iterator>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <type_traits>

namespace  Utils {

	class IOIterator
	{
	public:
		IOIterator() = default;
		virtual ~IOIterator() = default;

		//only suitable for trivial class/structs
		template <typename T>
		static bool binary(std::string filepath, std::vector<std::unique_ptr<T>>& vector, bool append=false)
		{
			//try to open the stream for writing
			std::ofstream stream;

			try {
				if(append)
					stream.open(filepath, std::ios::binary | std::ios::out | std::ios::app);
				else
					stream.open(filepath, std::ios::binary | std::ios::out);
			}
			catch (std::exception e)
			{
				std::ostringstream err;
				err << "Could not open " << filepath << " for writing using IOIterator: ";
				throw std::runtime_error(err.str().c_str());
			}
			
			try {
	
				for (auto& val : vector) {

					//convert the current pointer to a single byte pointer and write the size of the whole T class to disk
					stream.write((char*)val.get(), sizeof(*val));

				}
			}
			catch (std::exception e)
			{
				//clean up
				stream.close();

				throw std::runtime_error("Could not write objects using IOIterator.");
			}

			//clean up
			stream.close();

			return true;
		}

		template <class T>
		static std::vector<std::unique_ptr<T>> vectorFromBinary(std::string filepath, int objSize = 0)
		{

			if (objSize == 0)
				objSize = sizeof T;

			//try to open the stream for reading
			std::ifstream stream;

			try {
				stream.open(filepath, std::ios::binary | std::ios::in);
			}
			catch (std::exception e)
			{
				std::ostringstream err;
				err << "Could not open " << filepath << " for reading using IOIterator: ";
				throw std::runtime_error(err.str().c_str());
			}

			auto results = std::vector<std::unique_ptr<T>>();
		
			while (!stream.eof())
			{		
				//create the object
				std::unique_ptr<T> obj = std::make_unique<T>();

				//read from binary
				if (!stream.read(reinterpret_cast<char*>(obj.get()), objSize))
					break;

				results.push_back(std::move(obj));
			}

			//clean up
			stream.close();

			return std::move(results);

		}

		template <class T>
		static std::vector<std::unique_ptr<T>> vectorFromCsv(std::string filepath, bool hasHeader = false)
		{
			// Open an existing file 
			std::ifstream file(filepath);

			// Read the Data from the file 
			std::string line;

			//create a vector to hold the data
			std::vector<std::unique_ptr<T>> results;

			//throw away the header
			if(hasHeader)			
				std::getline(file, line, '\n');

			while (getline(file, line, '\n'))
			{
				results.push_back(std::make_unique<T>(line));
			}

			file.close();

			return results;
		}
	};

	
	

}
