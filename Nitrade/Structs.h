#pragma once


namespace Nitrade {

	const char DEFAULT_ASSET_DETAILS_CSV[] = "AssetData\\Assets.csv";
	enum tradeDirection { Long = 1, Short = -1 };

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
		int tradeId{ 0 };
		int variantId{ 0 };
		float openLevel{ 0 }; //the price the trade exectuted at
		float closeLevel{ 0 }; //the price the trade closed at
		float commission{ 0 }; //dollars paid in commission
		float spread{ 0 }; //spread in points
		float profit{ 0 }; //trade profit in cents
		int size{ 0 }; //size in units ie. for forex 0.01 lots would be size 1000
		float stopLoss{ 0 };
		float takeProfit{ 0 };
		long long openTime{ 0 }; //time the trade opened
		long long closeTime{ 0 }; //time the trade closed
		char assetName[10]{ "" };
		tradeDirection direction{ tradeDirection::Long };
	};


#pragma pack(pop)

}
