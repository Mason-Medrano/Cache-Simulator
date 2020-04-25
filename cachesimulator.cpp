#include <iostream>
#include "MainMemory.h"
#include "Cache.h"
#include "Simulator.h"
using namespace std;

int main(int argc, char* argv[]) {
	
	Simulator::PromptMenu();
	/*
	MainMemory RAM(argv[1]);
	Cache Cache(32, 8, 2, 1, 2, 1, &RAM);

	Cache.CacheWrite("00000000", "AB", "0x0");

	cout << endl << endl << endl;

	Cache.CacheWrite("00001100", "3434", "0xC");

	cout << endl << endl << endl;

	Cache.CacheWrite("00010001", "5555", "0x11");

	cout << endl << endl << endl;

	Cache.CacheWrite("00011100", "45", "0x1C");

	cout << endl << endl << endl;

	Cache.CacheWrite("00100000", "$$#", "0x20");

	Cache.PrintCacheContents();

	cout << endl << endl << endl;

	RAM.PrintMemoryContents();
*/
	return 0;
}