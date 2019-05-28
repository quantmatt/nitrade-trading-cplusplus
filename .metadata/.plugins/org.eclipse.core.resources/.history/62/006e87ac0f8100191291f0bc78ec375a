// Tester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <time.h>
#include <iostream>

#include "HistoricSimulator.h"
#include "TestStrategy.h"

using namespace Nitrade;
using namespace std;

int main()
{
	

	clock_t t;
	t = clock();

	//create a simulator
	HistoricSimulator hs;

	//setup a strategy and pass it to the simulator
	unique_ptr<Strategy> s = make_unique<TestStrategy>();
	hs.setup(move(s));

	//setup some assets to test
	std::vector<std::string> assets;
	assets.push_back("EURJPY");
	assets.push_back("GBPJPY");
	assets.push_back("CADJPY");
	assets.push_back("AUDJPY");
	assets.push_back("NZDJPY");
	assets.push_back("CHFJPY");
	assets.push_back("USDJPY");
	assets.push_back("EURUSD");

	//optimise strategy using 6 cores
	hs.optimise(6, assets);

	t = clock() - t;

	cout << t << endl;
}