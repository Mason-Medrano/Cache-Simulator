#include <iostream>
#include<string>

#include "MainMemory.h"
#include "Cache.h"
#include "Simulator.h"
using namespace std;




Simulator::Simulator(MainMemory* inputRAM, Cache* inputCache){
    RAM = *inputRAM;
	cache = *inputCache;
	HexToBinary["0"] = "0000";
	HexToBinary["1"] = "0001";
	HexToBinary["2"] = "0010";
	HexToBinary["3"] = "0011";
	HexToBinary["4"] = "0100";
	HexToBinary["5"] = "0101";
	HexToBinary["6"] = "0110";
	HexToBinary["7"] = "0111";
	HexToBinary["8"] = "1000";
	HexToBinary["9"] = "1001";
	HexToBinary["A"] = "1010";
	HexToBinary["B"] = "1011";
	HexToBinary["C"] = "1100";
	HexToBinary["D"] = "1101";
	HexToBinary["E"] = "1110";
	HexToBinary["F"] = "1111";
}

void Simulator::executeCommand(){
    string command = "";
	while(command != "quit"){
		cout << "*** Cache simulator menu ***" << endl
		<< "type one command: " << endl 
		<< "1. cache-read" << endl
		<< "2. cache-write" << endl
		<< "3. cache-flush" << endl
		<< "4. cache-view" << endl
		<< "5. memory-view" << endl
		<< "6. cache-dump" << endl
		<< "7. memory-dump" << endl
		<< "8. quit" << endl
		<< "*******************" << endl;
		cin >> command;

		if(command == "cache-read"){
			cin >> command;
			command = command.substr(2, command.size() - 1);
			string binary = "";
			for(int i = 0; i < command.size(); i++){
				binary = binary +  HexToBinary[command.substr(i,1)];
			}
			cache.CacheRead(binary, command);
		}else if(command == "cache-write"){
			//not implemented yet
		}else if(command == "cache-flush"){
			cache.CacheFlush();
			cout << "cache_cleared" << endl;
		}else if(command == "cache-view"){
			cache.PrintCacheContents();
		}else if(command == "memory-view"){
			RAM.PrintMemoryContents();
		}else if(command == "cache-dump"){
            cache.CacheDump();
		}else if(command == "memory-dump"){
			RAM.MemoryDump();
		}else if(command != "quit" ){
			cout << "That is an invalid command. Please type a valid command" << endl;
		}
	}
}

void Simulator::PromptMenu(){
    MainMemory RAM("input.txt");
    MainMemory* memory = &RAM;
    int cache_size = 0;
    int data_block = 0;
    int assoc = 0;
    int replacement = 0;
    int write_hit = 0;
    int write_miss = 0;

    cout << endl << "configure the cache: " << endl;
    cout << "cache size: ";
    cin >> cache_size;
    cout << "data block size: ";
    cin >> data_block;
    cout << "associativity: ";
    cin >> assoc;
    cout << "replacement policy: ";
    cin >> replacement;
    cout << "write hit policy: ";
    cin >> write_hit;
    cout << "write miss policy: ";
    cin >> write_miss;
   
    Cache NewCache(cache_size, data_block, assoc, replacement, write_hit, write_miss, memory);
    cout << "cache successfully configured!" << endl;
    Cache* cache_ptr = &NewCache;

    Simulator Sim(memory, cache_ptr);
    Sim.executeCommand();


}