// File: Cache.h
// Author(s): Shane McGookey & Mason Medrano
// Date: 04/22/2020
// Section: CSCE 312-511 (Both)
// E-mail: Shane_McGookey@tamu.edu & masonmedrano@tamu.edu
// Description:
// This file contains the class declarations for the Cache
// object. The Cache object implements reading, writing, and
// any policies associated with its operations.

#ifndef _CACHE_H_
#define _CACHE_H_

#include <vector>
#include <utility>
#include <string>
#include "CacheLine.h"
#include "MainMemory.h"

class Cache {
public:
	/*Default and parameterized constructors for the cache.
	  Takes in the users input and calculates number of
	  tag bits, block offset bits, etc.*/
	Cache();
	Cache(int cacheSize, int blockSize, int linesPerSet, int replacePolicyInput, int writeHPInput, int writeMPInput, MainMemory* ptrToRAM);

	/*Implementation of the cache-read funtion from the project
	  documentation. Takes in a binary address and its hexadecimal
	  equivalent, and outputs data for the given address. The given
	  data is read using the policy input from the user.*/
	std::string CacheRead(std::string binaryAddress, std::string hexAddressToPrint);

	/*Implementation of the cache-write function from the project
	  documentation. Takes in a binary address, its hexadecimal
	  equivalent, and the data to write. Given data is written
	  using the policy input by the user.*/
	void CacheWrite(std::string binaryAddress, std::string hexToStore, std::string hexAddressToPrint);

	/*Implementation of the cache-flush function from the project
	  documentation. Updates RAM and clears cache.*/
	void CacheFlush();

	/*Implementation of the cache-dump function. Outputs cache
	  contents to the cache.txt file.*/
	void CacheDump();

	/*Implementation of the cache-view function. Outputs cache
		  contents to the console.*/
	void PrintCacheContents();

private:
	int C; //Cache size in bytes
	int E; //Number of lines per set
	int S; //Number of sets
	int B; //Block size in bytes
	int s; //number of set index bits
	int b; //number of block offset bits
	int t; //number of tag bits
	int replacePolicy;   //replacement policy used
	int writeHitPolicy;  //write hit policy used
	int writeMissPolicy; // write miss policy used
	int cacheHits;       //number of cache hits
	int cacheMisses;     //number of cache misses

	/*Entire cache structure consisting of a 2-D vector of pairs.
	  Each pair contains a tag bit, represented as a string, and
	  a CacheLine object.*/
	std::vector<std::vector<std::pair<std::string, CacheLine>>> fullCache;

	/*Pointer to RAM object to be used in operations*/
	MainMemory* RAM;

	/*Helper function to convert binary strings
	  of numbers to its decimal equivalent. Returns
	  an integer.*/
	int BinaryToDecimal(std::string binaryNumber);

	/*Helper function to convert binary strings of
	  numbers to its hexadecimal equivalent. Returns
	  a string.*/
	std::string BinaryToHex(std::string binaryNumber);

	/*Helper function which capitalizes a string. Uses
	  a pass by reference*/
	void MakeStringUppercase(std::string& stringToChange);

	/*Takes a binary address to index where to read from
	  in the cache*/
	int ConvertToRAMAddress(std::string addressString);
};
#endif // !_CACHE_H_

