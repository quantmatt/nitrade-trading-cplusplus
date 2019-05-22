#pragma once


namespace Nitrade {

	const char DEFAULT_ASSET_DETAILS_CSV[] = "AssetData\\Assets.csv";
	enum tradeDirection { Long = 1, Short = -1 };
	enum exitType { ByStrategy, StopLoss, TakeProfit };

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

	struct RunningPL {
		char assetName[10]{ "" };
		int variantId{ 0 };
		long long timestamp{ 0 };
		double realisedProfit{ 0 };
		double unrealisedProfit{ 0 };
	};

	struct Trade {
		int tradeId{ 0 };
		int variantId{ 0 };
		float openLevel{ 0 }; //the price the trade exectuted at
		float closeLevel{ 0 }; //the price the trade closed at
		float commission{ 0 }; //dollars paid in commission
		float spread{ 0 }; //spread in points
		float profit{ 0 }; //trade profit in cents
		float pip{ 0 };
		float pipCost{ 0 };
		int size{ 0 }; //size in units of minimum amount ie. for forex 0.01 lots would be size 1
		float stopLoss{ 0 };
		float takeProfit{ 0 };
		float stopLevel{ 0 };
		float takeProfitLevel{ 0 };
		long long openTime{ 0 }; //time the trade opened
		long long closeTime{ 0 }; //time the trade closed
		char assetName[10]{ "" };
		tradeDirection direction{ tradeDirection::Long };
		exitType exit{ exitType::ByStrategy };
		float calcPointsProfit() { return ((closeLevel - openLevel) * direction); }
		float calcPipsProfit() { return calcPointsProfit() / pip; }
		float calcProfit() { return (calcPipsProfit() * pipCost - commission) * size; }
	};


#pragma pack(pop)

}
