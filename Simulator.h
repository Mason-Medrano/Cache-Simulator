// File: Simulator.h
// Author(s): Shane McGookey & Mason Medrano
// Date: 04/22/2020
// Section: CSCE 312-511 (Both)
// E-mail: Shane_McGookey@tamu.edu & masonmedrano@tamu.edu
// Description:
// This is the header file containing class delcaration for 
// the simulator interface.

#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_
#include<map>
#include<string>
#include "Cache.h"

class Simulator {
public:

    /*Parameterized constructor which accepts pointers to
      RAM and Cache objects. Also initializes map for
      hexadecimal to binary.*/
    Simulator(MainMemory* inputRAM, Cache* inputCache);
    ~Simulator();

    /*Primary function which starts the program in main.
      Displays initial menus and allows user to configure
      cache.*/
    static void PromptMenu(std::string inputFile);

private:

    /*RAM and Cache objects to be used during operations.*/
    MainMemory* RAM;
    Cache* cache;

    /*Hexadecimal to binary map for converting hexadecimal
      strings of numbers to binary strings of numbers.*/
    std::map<std::string, std::string> HexToBinary;

    /*Helper function which allows user to select commands.
      The function then executes given command, and returns
      to while loop.*/
    void executeCommand();
};



#endif