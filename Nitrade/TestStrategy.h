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
			int bufferSize = (int)std::fmax(_parameters["Period2"], _parameters["Period1"]);
			_features["ASK_CLOSE"] = std::make_unique<Utils::SeriesBuffer<double>>(bufferSize);
		};

		void flip(tradeDirection openDirection, tradeDirection closeDirection)
		{
			float pip = getPip();

			//record some data with the trade to use for analysis
			_data["Last 4 bar Log Change"] = log(askClose() / askClose(3));
			_data["Pips from SMA"] = (askClose() - _features["SMA_Slow"]->get(0)) / pip;

			//set the stop loss and target to be equal
			float stopLoss = pip * 100;

			//close open trades then open a new trade
			closeTrades(closeDirection);
			openTrade(openDirection, 1, stopLoss, stopLoss);
		}

		void onBar() {
			
			//only calculate when we have enough bars in the lookback
			if (getBarIndex() <= _parameters["Period2"])
				return;

			if (getDatasetName() == "240min")
			{
				_features["ASK_CLOSE"]->add(askClose());

				double smaFast = Indicators::SMA((int)_parameters["Period1"], _features["ASK_CLOSE"].get());
				double smaSlow = Indicators::SMA((int)_parameters["Period2"], _features["ASK_CLOSE"].get());

				_features["SMA_Fast"]->add(smaFast);
				_features["SMA_Slow"]->add(smaSlow);

				if (Indicators::CrossOver(_features["SMA_Fast"].get(), _features["SMA_Slow"].get()))
				{
					

					if (smaFast > smaSlow)
					{
						flip(tradeDirection::Short, tradeDirection::Long);
					}
					else if (smaFast < smaSlow)
					{
						flip(tradeDirection::Long, tradeDirection::Short);
					}
				}
			}
			
		};
	};
}



