// File: cachesimulator.cpp
// Author(s): Shane McGookey & Mason Medrano
// Date: 04/22/2020
// Section: CSCE 312-511 (Both)
// E-mail: Shane_McGookey@tamu.edu & masonmedrano@tamu.edu
// Description: This file includes the main function for the
//              entire simulator. The main function simply prompts
//              the Simulator class' menu, and then the simulator
//              class is responsible for all functionality from
//              that point forwards.

#include <iostream>
#include <string>
#include "MainMemory.h"
#include "Cache.h"
#include "Simulator.h"
using namespace std;

int main(int argc, char* argv[]) {
	
	// Make sure that the user has specified
	// an input file.
	if (argv[1] == "") {
		cerr << "Please specify an input file." << endl;
		exit(1);
	}
	
	// Prompt the simulator's menu;
	// will start out with allowing
	// the user to configure the
	// cache.
	Simulator::PromptMenu(argv[1]);

	// Return 0 upon successful
	// program completion.
	return 0;
}