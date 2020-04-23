#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_
#include "Cache.h"

class Simulator{
public:
    Simulator(MainMemory* inputRAM, Cache* inputCache);
    static void PromptMenu();

private:
    MainMemory RAM{};
    Cache cache{};
    void executeCommand();
};



#endif