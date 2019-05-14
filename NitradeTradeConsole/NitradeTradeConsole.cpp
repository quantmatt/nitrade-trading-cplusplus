// NitradeTradeConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include <memory>
#include "NitradeLib.h"

using namespace std;
using namespace Nitrade;

int main()
{
	{
		/*
		testClass* ab = new testClass();
		ab->func();

		//create the controller object that holds all the asset, trade and strategy data
		Controller controller;

		//the assets to use
		string assetNames[] = { "EURUSD" };

		//add each asset to the controller with a binary reader attached
		for (int i = 0; i < sizeof(assetNames) / sizeof(string); i++)
		{
			//create a binary chunk reader for the asset
			string filepath = "D:\\TickData\\" + assetNames[0] + "_m1.bin";
			
			std::unique_ptr<BinaryChunkReader> bcr = std::make_unique<BinaryChunkReader>(filepath);			
			std::unique_ptr<IAsset> asset = std::make_unique<Asset>(assetNames[0], std::move(bcr));

			PriceData* pd = new PriceData(200, 60);
			asset->addPriceData("60min", pd);

			//PriceData* pd3 = new PriceData(200, 240);
			//asset->addPriceData("240min", pd3);

			controller.addAsset(std::move(asset));
		}

		//add in all strategy variations
		for (int i = 0; i < 100; i++)
		{
			Strategy* strategy = new Strategy();
			controller.addStrategy(strategy);
			strategy->addRequireData("EURUSD", "60min");
		}

		
		//Can optimise speed reading in chunks of different sizes
		Nitrade::BackTest backTest;
		backTest.RunAll(&controller);

*/

		Strategy s;
		clock_t t;
		t = clock();

		//BackTest backTest;
		//backTest.Optimise(s);

		t = clock() - t;
		std::cout << "time: " << t << " miliseconds" << std::endl;
		std::cout << CLOCKS_PER_SEC << " clocks per second" << std::endl;
		std::cout << "time: " << t * 1.0 / CLOCKS_PER_SEC << " seconds" << std::endl;
	}

	std::string test;
	std::cin >> test;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
