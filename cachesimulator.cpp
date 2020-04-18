#include <iostream>
#include "MainMemory.h"
#include "Cache.h"
using namespace std;

int main(int argc, char* argv[]) {
	MainMemory RAM(argv[1]);
	Cache Cache(32, 8, 2, 1, 1, 1, &RAM);

	cout << Cache.CacheRead("00000000", "0x0");

	return 0;
}