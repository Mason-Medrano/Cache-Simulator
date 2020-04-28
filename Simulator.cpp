#include <iostream>
#include <string>
#include <stdexcept>
#include "MainMemory.h"
#include "Cache.h"
#include "Simulator.h"

using std::cout;
using namespace std;

/*Parameterized constructor which accepts pointers to
	  RAM and Cache objects. Also initializes map for
	  hexadecimal to binary.*/
Simulator::Simulator(MainMemory* inputRAM, Cache* inputCache) {
	RAM = inputRAM; //sets RAM to input pointer
	cache = inputCache; //sets cache to input pointer

	//sets values of HextoBinary map
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

/*Helper function which allows user to select commands.
  The function then executes given command, and returns
  to while loop.*/
void Simulator::executeCommand() {

	//Loops simulator menu to end only when the user types 'quit'
	string command = "";
	string address = "";
	string data = "";
	while (command != "quit") {

		//Output for the siumlator menu
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
			<< "****************************" << endl;
		cin >> command;
		try {
			//Determines function to run based on user input
			if (command == "cache-read") {
				cin >> address;
				address = address.substr(2, address.size() - 1);
				string binary = "";
				for (int i = 0; i < address.size(); i++) {
					//uses hextobinary map to convert from hex to binary
					binary = binary + HexToBinary[address.substr(i, 1)];
				}
				//executes cache-read
				string data = cache->CacheRead(binary, address);
				cout << "data:0x" << data << endl;
			}
			else if (command == "cache-write") {
				cin >> address >> data;
				address = address.substr(2, address.size() - 1);
				data = data.substr(2, data.size() - 1);
				string binary = "";
				for (int i = 0; i < address.size(); i++) {
					//uses hextobinary map to convert from hex to binary
					binary = binary + HexToBinary[address.substr(i, 1)];
				}
				//executes cache-write
				cache->CacheWrite(binary, data, address);
			}
			else if (command == "cache-flush") {
				//executes cache-flush
				cache->CacheFlush();
			}
			else if (command == "cache-view") {
				//executes cache-view
				cache->PrintCacheContents();
			}
			else if (command == "memory-view") {
				//executes memory-view
				RAM->PrintMemoryContents();
			}
			else if (command == "cache-dump") {
				//executes cache-dump
				cache->CacheDump();
			}
			else if (command == "memory-dump") {
				//executes memory-dump
				RAM->MemoryDump();
			}
			else if (command == "quit") {
				exit(0);
			}
			else {
				throw std::invalid_argument("Must select a simulator command.");
			}
		}
		catch (std::invalid_argument & invalid) {
			cerr << invalid.what();
			exit(1);
		}
	}
}
/*Primary function which starts the program in main.
	  Displays initial menus and allows user to configure
	  cache.*/
void Simulator::PromptMenu() {
	//constructs RAM with input file
	cout << "*** Welcome to the cache simulator ***" << endl;
	MainMemory RAM("input.txt");
	MainMemory* memory = &RAM;

	//initializes variables for user input
	int cache_size = 0;
	int data_block = 0;
	int assoc = 0;
	int replacement = 0;
	int write_hit = 0;
	int write_miss = 0;

	//gathers user input
	try {
		cout << "configure the cache: " << endl;
		cout << "cache size: ";
		cin >> cache_size;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			std::cout << "Invalid input.";
			exit(1);
		}
		else if (cache_size < 8 || cache_size > 256) {
			throw std::invalid_argument("Cache size must be between 8 - 256 Bytes.");
		}

		cout << "data block size: ";
		cin >> data_block;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			std::cout << "Invalid input.";
			exit(1);
		}

		cout << "associativity: ";
		cin >> assoc;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			std::cout << "Invalid input.";
			exit(1);
		}
		else if (assoc != 1 && assoc != 2 && assoc != 4) {
			throw std::invalid_argument("Associativity must be 1, 2, or 4.");
		}

		cout << "replacement policy: ";
		cin >> replacement;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			std::cout << "Invalid input.";
			exit(1);
		}
		else if (replacement != 1 && replacement != 2) {
			throw std::invalid_argument("Replacement policy must be 1 or 2.");
		}

		cout << "write hit policy: ";
		cin >> write_hit;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			std::cout << "Invalid input.";
			exit(1);
		}
		else if (write_hit != 1 && write_hit != 2) {
			throw std::invalid_argument("Write hit policy must be 1 or 2.");
		}

		cout << "write miss policy: ";
		cin >> write_miss;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			std::cout << "Invalid input.";
			exit(0);
		}
		else if (write_miss != 1 && write_miss != 2) {
			throw std::invalid_argument("Write miss policy must be 1 or 2.");
		}
	}
	catch (std::invalid_argument & invalid) {
		cerr << invalid.what();
		exit(1);
	}


	//constructs cache with given user input
	Cache NewCache(cache_size, data_block, assoc, replacement, write_hit, write_miss, memory);
	cout << "cache successfully configured!" << endl;
	Cache* cache_ptr = &NewCache;

	//initializes simulator object with RAM and Cache
	Simulator Sim(memory, cache_ptr);

	//calls helper function to select commands
	Sim.executeCommand();
}