#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_
#include<map>
#include<string>
#include "Cache.h"

class Simulator{
public:
    Simulator(MainMemory* inputRAM, Cache* inputCache);
    static void PromptMenu();

private:
    MainMemory RAM{};
    Cache cache{};
    std::map<std::string, std::string> HexToBinary;
    std::map<std::string,std::string>::iterator it;
    void executeCommand();
};



#endif