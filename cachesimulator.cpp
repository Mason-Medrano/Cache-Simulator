#include "MainMemory.h"
using namespace std;

int main(int argc, char* argv[]) {
	MainMemory RAM(argv[1]);
	RAM.PrintMemoryContents();
	return 0;
}