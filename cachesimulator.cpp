#include <iostream>
#include <string>
#include "MainMemory.h"
#include "Cache.h"
//#include "Simulator.h"
using namespace std;

void MakeSpace() {
	cout << endl << endl;
}

int main(int argc, char* argv[]) {
	//Simulator::PromptMenu();

	string dataReturned = "";
	
	MainMemory RAM(argv[1]);
	Cache Cache(32, 8, 2, 2, 1, 1, &RAM);

	Cache.CacheWrite("10011011", "B5", "0x9B");

	MakeSpace();

	Cache.PrintCacheContents();

	MakeSpace();

	RAM.PrintMemoryContents();

	return 0;
}