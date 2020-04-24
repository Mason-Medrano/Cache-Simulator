#include <iostream>
#include<string>
#include "MainMemory.h"
#include "Cache.h"
#include "Simulator.h"
using namespace std;

Simulator::Simulator(MainMemory* inputRAM, Cache* inputCache){
    RAM = *inputRAM;
	cache = *inputCache;
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
			//invalid command
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