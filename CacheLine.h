// File: CacheLine.h
// Author(s): Shane McGookey & Mason Medrano
// Date: 04/22/2020
// Section: CSCE 312-511 (Both)
// E-mail: Shane_McGookey@tamu.edu & masonmedrano@tamu.edu
// Description:
// This file contains the class declarations for the CacheLine
// object. The CacheLine object represents a single line in a
// set of the cache.
#ifndef _CACHE_LINE_H_
#define _CACHE_LINE_H_

#include <string>
#include <vector>
#include <chrono>
#include <fstream>

class CacheLine {
public:

	/*Default and parameterized constructors for 
	  Cacheline object using block size or a 
	  vector of strings.*/
	CacheLine();
	CacheLine(int blockSize);
	CacheLine(std::vector<std::string> dataToSet);

	/*Returns a string from a line in cache.
	  The block offset bits are used to index.*/
	std::string ReadFromCacheLine(int blockOffset);

	/*Writes a string to an address in cache. The block
	  offset bits are used to index.*/
	void WriteToCacheLine(int blockOffset, std::string dataToWrite);

	/*Returns a vector of strings representing the
	  data in the cache line.*/
	std::vector<std::string> GetCacheLine();

	/*Writes a vector of strings to the cache line.*/
	void SetData(std::vector<std::string> dataToSet);

	/*Outputs to console the data in the cache line.*/
	void PrintCacheLine();

	/*Outputs to a file the data in the cache line.*/
	void PrintCacheLineToFile(std::ofstream& ofs);

    /*Sets the valid bit of the cache line to 1.*/
	void AssertValidBit();

	/*Sets the dirty bit of the cache line
	  to the specified value.*/
	void SetDirtyBit(int bit);

	/*Sets timer object to current time. This is 
	  used in the LRU policy.*/
	void SetTimer();

	/*Sets the source addres to the specified value.*/
	void SetSourceAddress(int sourceAddressInput);

	/*Returns the source address.*/
	int GetSourceAddress();

	/*Returns the valid bit.*/
	int GetValidBit();

	/*Returns the dirtiy bit.*/
	int GetDirtyBit();

	/*Returns the time object.*/
	std::chrono::steady_clock::time_point GetTime();
private:
	/*Integers representing valid and dirty bit.*/
	int validBit;
	int dirtyBit;

	/*Chrono object used for keeping track of time.
	  Used in LRU policy.*/
	std::chrono::steady_clock::time_point timeInCache;

	/*Vector of strings representing the data in the cache.*/
	std::vector<std::string> data;

	/*Integer representing the original address in RAM.*/
	int sourceAddress;
};

#endif // !_LINE_H_
