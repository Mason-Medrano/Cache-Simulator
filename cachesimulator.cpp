#include <iostream>
#include "MainMemory.h"
#include "Cache.h"
#include "Simulator.h"
using namespace std;

int main(int argc, char* argv[]) {

	Simulator::PromptMenu();
	/*
	MainMemory RAM(argv[1]);
	Cache Cache(32, 8, 2, 2, 1, 1, &RAM);

	cout << Cache.CacheRead("00000000", "0x0");
	cout << Cache.CacheRead("00001000", "0x8");
	cout << Cache.CacheRead("00010000", "0x10");
	cout << Cache.CacheRead("00011000", "0x18");
	cout << Cache.CacheRead("00100000", "0x20");
	cout << Cache.CacheRead("00110000", "0x30");

	cout << endl << endl << endl;
	Cache.PrintCacheContents();

	Cache.CacheDump();
	return 0;
	*/
}