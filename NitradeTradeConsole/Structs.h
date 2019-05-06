#pragma once


namespace Nitrade {

#pragma pack(push, 1)
	struct Bar {
		long long timestamp;
		float bidOpen;
		float bidClose;
		float bidHigh;
		float bidLow;
		float askOpen;
		float askClose;
		float askHigh;
		float askLow;
		int volume;
	};

	struct Trade {
		long long openTime; //time the trade opened
		long long closeTime; //time the trade closed
		float openLevel; //the price the trade exectuted at
		float closeLevel; //the price the trade closed at
		int commission; //cents paid in commission
		int spread; //spread in points
		int profit; //trade profit in cents
		int size; //size in units ie. for forex 0.01 lots would be size 1000
		bool isLong; //true if long false if short
	};

#pragma pack(pop)

}
