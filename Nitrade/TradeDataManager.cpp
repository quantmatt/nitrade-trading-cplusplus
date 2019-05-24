#include "TradeDataManager.h"
#include <fstream>
#include <sstream>

void Nitrade::TradeDataManager::push_back(int tradeId, std::map<std::string, double>& data)
{
	//copy the trade data to the map
	_tradeData[tradeId] = data;

	//add any fields to the fields map (either overwrite or insert)
	for(auto value : data)	
		_fieldNames[value.first] = true;
}

bool Nitrade::TradeDataManager::toBinary(std::string filepath, int startTradeId)
{
	//write meta data at the start of the file
	//int - number of fields
	//char[30] - for each field name
	//followed by each set of trade data
	//int tradeId, double or null for each field


	//try to open the stream for writing
	std::ofstream stream;

	try {
		stream.open(filepath, std::ios::binary | std::ios::out | std::ios::app);
	}
	catch (std::exception e)
	{
		std::ostringstream err;
		err << "Could not open " << filepath << " for writing additional trade data: ";
		throw std::exception(err.str().c_str());
	}

	try {

		if (startTradeId == 0) //only needs to be done for the first tradeManager write - others will append to this file
		{
			//write the number of fields
			int fieldCount = _fieldNames.size();
			stream.write((char*)& fieldCount, sizeof(int));

			//write each field name
			for (auto& val : _fieldNames)
			{
				//need to make sure all strings are fixed length of 30 bytes

				//copy the name of the field
				std::string name = val.first;

				//resize it to a 30 char array in total
				//fill the reset with white space
				name.resize(30, ' ');

				//write the 30 byte string name
				stream.write((char*)name.c_str(), 30);
			}
		}


		for (auto& trade : _tradeData) {

			auto& tradeData = trade.second;
			int tradeId = trade.first + startTradeId;

			//write the tradeId
			stream.write((char*)& tradeId, sizeof(int));

			//now write all the data in the order of the field names 
			for (auto& fields : _fieldNames)
			{
				auto& fieldName = fields.first;
				double nullValue = 0;				

				//try to find the field in this trade data because some trades might not have all
				//the info
				if(!(tradeData.find(fieldName) == tradeData.end()))
					stream.write((char*)(&tradeData[fieldName]), sizeof(double));
				else
					stream.write((char*)(&nullValue), sizeof(double));
			}

		}
	}
	catch (std::exception e)
	{
		//clean up
		stream.close();

		throw std::exception("Could not write Trade Data.");
	}

	//clean up
	stream.close();

	return false;
}
