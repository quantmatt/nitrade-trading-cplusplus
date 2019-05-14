#pragma once
#include "Strategy.h"

namespace Nitrade {
	class TestStrategy :
		public Strategy
	{
	private:
		std::string _name{ "Strategy" };

	public:
		TestStrategy() = default;
		virtual ~TestStrategy() = default;

		std::unique_ptr<Strategy> clone() {
			return std::make_unique<TestStrategy>();
		}

		void onInit() {
			_features["SMA_Fast"] = std::make_unique<Utils::SeriesBuffer<double>>(2);
			_features["SMA_Slow"] = std::make_unique<Utils::SeriesBuffer<double>>(2);
			_features["ASK_CLOSE"] = std::make_unique<Utils::SeriesBuffer<double>>(_parameters["Period2"]);
		};

		void onBar() {
			
			//only calculate when we have enough bars in the lookback
			if (getBarIndex() <= _parameters["Period2"])
				return;

			if (getDatasetName() == "60min")
			{
				_features["ASK_CLOSE"]->add(askClose());

				double smaFast = Indicators::SMA(_parameters["Period1"], _features["ASK_CLOSE"].get());
				double smaSlow = Indicators::SMA(_parameters["Period2"], _features["ASK_CLOSE"].get());

				_features["SMA_Fast"]->add(smaFast);
				_features["SMA_Slow"]->add(smaSlow);

				if (Indicators::CrossOver(_features["SMA_Fast"].get(), _features["SMA_Slow"].get()))
				{
					double stopLoss = getPip() * 10;

					if (smaFast > smaSlow)
					{
						closeTrades(tradeDirection::Short);
						openTrade(tradeDirection::Long, 1, stopLoss, stopLoss);
					}
					else if (smaFast < smaSlow)
					{
						closeTrades(tradeDirection::Long);
						openTrade(tradeDirection::Short, 1, stopLoss, stopLoss);
					}
				}
			}
			
		};
	};
}



