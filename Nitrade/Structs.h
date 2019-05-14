#pragma once


namespace Nitrade {

	enum tradeDirection { Long, Short };

#pragma pack(push, 1)
	struct Bar {
		long long timestamp{ 0 };
		float bidOpen{ 0 };
		float bidClose{ 0 };
		float bidHigh{ 0 };
		float bidLow{ 0 };
		float askOpen{ 0 };
		float askClose{ 0 };
		float askHigh{ 0 };
		float askLow{ 0 };
		int volume{ 0 };
	};

	struct Trade {
		int tradeId;
		std::string assetName{ "" };
		int variantId{ 0 };
		tradeDirection direction{ tradeDirection::Long };
		long long openTime{ 0 }; //time the trade opened
		long long closeTime{ 0 }; //time the trade closed
		float openLevel{ 0 }; //the price the trade exectuted at
		float closeLevel{ 0 }; //the price the trade closed at
		int commission{ 0 }; //cents paid in commission
		int spread{ 0 }; //spread in points
		int profit{ 0 }; //trade profit in cents
		int size{ 0 }; //size in units ie. for forex 0.01 lots would be size 1000
		double stopLoss{ 0 };
		double takeProfit{ 0 };
	};


#pragma pack(pop)

}
