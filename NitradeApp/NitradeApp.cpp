#include <time.h>
#include <iostream>

#include "NitradeLib.h"
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
	assets.push_back("EURUSD");
	assets.push_back("AUDUSD");

	cout << "Starting optimisation on " << assets.size() << " assets" << endl;

	//optimise strategy using 6 cores
	hs.optimise(6, assets);
	

	t = clock() - t;

	cout << "Took " << (t / CLOCKS_PER_SEC) << " seconds"  << endl;
	cout << "Trades generated and saved under trades.bin" << endl;
}