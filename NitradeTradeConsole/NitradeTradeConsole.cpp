// NitradeTradeConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>

#include "BackTest.h"
#include "BinaryChunkReader.h"
#include "Controller.h"

int main()
{
    

	//create a binary chunk reader for the eurusd asset
	std::string filepath = "D:\\TickData\\EURUSD_m1.bin";
	Nitrade::BinaryChunkReader bcr(filepath);
	Nitrade::Controller controller;
	controller.setBinaryReader(&bcr);
	
	
	clock_t t;
	t = clock();
	//Can optimise speed reading in chunks of different sizes
	Nitrade::BackTest backTest;
	backTest.Run(&controller, "EURUSD");
	
	/*
	int loops = 100;
	double tally = 0;
	for (int i = 0; i < loops; i++)
		tally += backTest.ChunkArray(50);
	double av = tally / loops;

	std::cout << "----------------------------------" << std::endl;
	std::cout << "Av time: " << av << std::endl;
	//backTest.BarAtATime();
	*/
	
	t = clock() - t;
	std::cout << "time: " << t << " miliseconds" << std::endl;
	std::cout << CLOCKS_PER_SEC << " clocks per second" << std::endl;
	std::cout << "time: " << t * 1.0 / CLOCKS_PER_SEC << " seconds" << std::endl;

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
