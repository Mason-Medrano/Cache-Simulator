// File: Cache.cpp
// Author(s): Shane McGookey & Mason Medrano
// Date: 04/22/2020
// Section: CSCE 312-511 (Both)
// E-mail: Shane_McGookey@tamu.edu & masonmedrano@tamu.edu
// Description: This file includes the function definitions
//              for the Cache class. These function definitions
//              allow the caller to read from cache, write to 
//              cache, print the contents of the cache (to the
//              console or to a file), and clear the cache.

#include <math.h>
#include <iostream>
#include <iomanip> 
#include <sstream>
#include <fstream>
#include <stdlib.h> 
#include <chrono>
#include "Cache.h"
using namespace std;

// The length of the address
// in bits, as per project
// specifications.
const int addressLength = 8;

// The default constructor for
// the Cache class. Constructs
// a Cache object with default
// flags for each data member,
// and a non-associative (NULL) RAM.
Cache::Cache() 
	: C(-1),  E(-1), B(-1), replacePolicy(-1), writeHitPolicy(-1), 
	writeMissPolicy(-1), RAM(NULL), cacheHits(0), cacheMisses(0)
{
	// All operations are completed in the
	// constructor initializer list.
}

// The custom constructor for
// the Cache class. Allows the
// caller to specify the size of
// the cache, the data block size,
// the replacement policy, write-hit
// policy, and write-miss policy.
// Sets both cacheHits and cacheMisses
// to 0, and takes the RAM to be associated
// with the cache as an argument.
Cache::Cache(int cacheSize, int blockSize, int linesPerSet, int replacePolicyInput, int writeHPInput, int writeMPInput, MainMemory* ptrToRAM)
	: C(cacheSize), E(linesPerSet), B(blockSize), replacePolicy(replacePolicyInput), writeHitPolicy(writeHPInput), 
	  writeMissPolicy(writeMPInput), RAM(ptrToRAM), cacheHits(0), cacheMisses(0)
	 
{
	// The number of sets in the
	// cache is equal to the total
	// cache size divided by the
	// number of lines per set *
	// the block size.
	S = C / (E * B);
	// The number of set index bits
	// is equal to log_2(# of sets).
	s = log2(S);
	// The number of block offset bits
	// is equal to log_2(block size).
	b = log2(B);
	// The number of tag bits is
	// equal to the total length of the
	// address minus the set index and 
	// block offset bits.
	t = addressLength - (s + b);

	// Now that we know the number of
	// sets in the cache, resize the cache.
	fullCache.resize(S);

	// Sets all cache lines to their
	// default state.
	for (int i = 0; i < fullCache.size(); ++i) {
		fullCache.at(i) = vector<pair<string, CacheLine>>(E, pair<string, CacheLine>("", (B)));
	}
}

// Allows the caller to read a line from
// the cache. The function will handle both
// cache hits and misses; implementing the 
// caller-defined replacement policy
// in the event of a cache miss.
std::string Cache::CacheRead(string binaryAddress, string hexAddressToPrint)
{
	// Used to format the tag
	// bits in hexadecimal format.
	ostringstream temp;
	// The tag bits in hexadecimal
	// format.
	string hexTag = "";
	// The line of data read from
	// main memory (RAM).
	vector<string> readLine;
	// Tracks whether the line
	// being read was found in the
	// cache.
	bool foundLine = false;
	// Tracks whether the specific
	// set we are reading from is full.
	bool setFull = false;
	// The index of the line
	// being read from if it is
	// found in cache.
	int lineFoundIndex = 0;
	
	// Extract the tag bits from the binary string.
	string tagBits = binaryAddress.substr(0, t);
	// Extract the set index bits from the binary string.
	string setIndex = binaryAddress.substr((0 + t), s);
	// Extract the block offset bits from the binary string.
	string blockOffSet = binaryAddress.substr((0 + t + s), b);

	// Convert the binary set index bits to a decimal index.
	int setIndexNumeric = BinaryToDecimal(setIndex);
	// Convert the block offset bits to an integer.
	int blockOffsetNumeric = BinaryToDecimal(blockOffSet);
	// Convert the tags bits to an integer.
	int tagBitsNumeric = BinaryToDecimal(tagBits);
	// Convert the entire memory address to an integer.
	int memoryAddress = BinaryToDecimal(binaryAddress);
	// Used to determine the address in RAM that we
	// will start pulling from if the byte we are
	// reading needs to be moved from RAM to the cache.
	int adjustedMemAddress = ConvertToRAMAddress(binaryAddress);

	// Convert the tag bits to a hexadecimal format.
	temp << hex << tagBitsNumeric;
	hexTag = temp.str();
	// Make the hexadecimal uppercase.
	for (int i = 0; i < hexTag.length(); ++i) {
		hexTag.at(i) = toupper(hexTag.at(i));
	}

	// Print the set as a decimal,
	// and the tag as a hexadecimal.
	cout << "set:" << setIndexNumeric << endl;
	cout << "tag:" << hexTag << endl;

	// Determine the specific set we are working
	// with by using the integer version of the
	// set index bits. Note that fullCache's first
	// dimension is the sets, and the internal vector
	// stored within each set contains pairs of both
	// tag bits (to identify a line) and cache lines.
	std::vector<std::pair<string, CacheLine>>* set;
	set = &(fullCache.at(setIndexNumeric));

	// Check if we have the line we are
	// looking to read in the cache. If the
	// line's valid bit is 1 and the tag bits
	// match, then we have found a match.
	for (int i = 0; i < set->size(); ++i) {
		if (set->at(i).first == tagBits && set->at(i).second.GetValidBit() == 1) {
			foundLine = true;
			lineFoundIndex = i;
			break;
		}
	}

	// Executes if the line we are reading is not in the cache.
	if (!foundLine) {
		// Determine if the set is full, we can
		// do this by checking if the last line in
		// the set has a valid bit equal to 1.
		setFull = (set->at(set->size() - 1).second.GetValidBit() == 1);
		// Denote a cache miss.
		cout << "hit:no" << endl;
		// Increment the number of cache misses.
		++cacheMisses;
		
		// Executes if our respective set is not full.
		if (!setFull) {
			// Read the line from RAM which contains
			// the byte of data we are attempting to read.
			readLine = RAM->ReadLine(adjustedMemAddress, B);
			
			// Used to determine which line we are
			// going to be pulling the byte of data
			// from in cache.
			int indexToReturn = 0;
			// Create a cache line with the data read from RAM.
			pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
			// Save the address that the data was pulled
			// from in case we are using a write-back policy.
			currentLine.second.SetSourceAddress(adjustedMemAddress);

			// Update the lines timer since we
			// are reading from (using) it.
			currentLine.second.SetTimer();

			// Determine where the next open position
			// is in the cache, and store the newly
			// created cache line in that position.
			for (int i = 0; i < set->size(); ++i) {
				if (set->at(i).second.GetValidBit() == 0) {
					indexToReturn = i;
					set->at(i) = currentLine;
					break;
				}
			}

			cout << "eviction_line:" << indexToReturn << endl;
			cout << "ram_address:0x" << hexAddressToPrint << endl;

			// Return the byte from the new cache line
			// specified by our block offset index.
			return set->at(indexToReturn).second.ReadFromCacheLine(blockOffsetNumeric);
		}
		// Executes if the cache is full and we are using
		// a random replacement policy.
		else if (replacePolicy == 1 && setFull) {
			// Seed the RNG.
			srand(time(0));
			// Determine the max index we can have for the set.
			int maxIndex = (set->size());
			// Generate a random index between 0 and (maxIndex - 1).
			int replacementIndex = rand() % maxIndex;
			cout << "eviction_line:" << replacementIndex << endl;
			cout << "ram_address:0x" << hexAddressToPrint << endl;

			// Read the line from RAM which contains
			// the byte of data we are attempting to read.
			readLine = RAM->ReadLine(adjustedMemAddress, B);
			// Create a cache line with the data read from RAM.
			pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));

			// Save the address that the data was pulled
			// from in case we are using a write-back policy.
			currentLine.second.SetSourceAddress(adjustedMemAddress);

			// If we are evicting a line whose dirty bit is set to 1, then
			// write that line back into the address that it was sourced from
			// in Main Memory.
			if (set->at(replacementIndex).second.GetDirtyBit() == 1) {
				RAM->WriteLine(set->at(replacementIndex).second.GetSourceAddress(), B, set->at(replacementIndex).second.GetCacheLine());
			}

			// Update the lines timer since we
			// are reading from (using) it.
			currentLine.second.SetTimer();

			// Replace the line at the randomly
			// generated index.
			set->at(replacementIndex) = currentLine;

			// Return the byte that was being read
			// after the block has been placed in cache.
			return set->at(replacementIndex).second.ReadFromCacheLine(blockOffsetNumeric);
		}
		// Executes if the cache is full and we are using
		// a LRU replacement policy.
		else if (replacePolicy == 2 && setFull) {
			// The index of the cache line
			// that was used the furthest in the past.
			int leastIndex = -1;
			// Set-up the cache line which
			// was used the furthest in the
			// past as the first line in the set.
			std::chrono::steady_clock::time_point leastTime = set->at(0).second.GetTime();
			leastIndex = 0;

			// Iterate through all other lines
			// in the set to determine if there
			// is another line that was used earlier.
			// Change leastIndex accordingly.
			for (int i = 1; i < set->size(); ++i) {
				if (set->at(i).second.GetTime() < leastTime) {
					leastTime = set->at(i).second.GetTime();
					leastIndex = i;
				}
			}

			// The line we are evicting is the cache
			// line used the furthest in the past.
			cout << "eviction_line:" << leastIndex << endl;
			cout << "ram_address:0x" << hexAddressToPrint << endl;

			// Read the data from RAM that we will be
			// putting into the cache.
			readLine = RAM->ReadLine(adjustedMemAddress, B);

			// Create the cache line to be inserted.
			pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
			// Save the line's source address in case
			// we are using write-back.
			currentLine.second.SetSourceAddress(adjustedMemAddress);
			// Write the evicted line back to RAM if its
			// dirty bit was equal to 1.
			if (set->at(leastIndex).second.GetDirtyBit() == 1) {
				RAM->WriteLine(set->at(leastIndex).second.GetSourceAddress(), B, set->at(leastIndex).second.GetCacheLine());
			}

			// Set the timer on the new line since it
			// is being read from.
			currentLine.second.SetTimer();

			// Update the line.
			set->at(leastIndex) = currentLine;

			// Return the byte that was being read.
			return set->at(leastIndex).second.ReadFromCacheLine(blockOffsetNumeric);
		}
	}
	// Executes if the line we are reading is
	// in the cache.
	else {
		// Denote a cache hit and
		// no eviction line.
		cout << "hit:yes" << endl;
		cout << "eviction_line:-1" << endl;
		cout << "ram_address:-1" << endl;
		// Increment the number of cache
		// hits.
		++cacheHits;
		// Update the accessed cache line's
		// timer since we just read from (used)
		// it.
		set->at(lineFoundIndex).second.SetTimer();
		// Return the byte within the line at
		// the integer index generated through
		// the block offset bits.
		return set->at(lineFoundIndex).second.ReadFromCacheLine(blockOffsetNumeric);
	}
}

// Allows the caller to write 
// to the cache; handles both
// write hits and write misses,
// implementing both various
// replacement policies and
// various write-hit and write-miss
// policies.
void Cache::CacheWrite(string binaryAddress, string hexToStore, string hexAddressToPrint)
{
	// Used to format the tag
	// bits in hexadecimal format.
	ostringstream temp;
	// The tag bits in hexadecimal
	// format.
	string hexTag = "";
	// The line of data written to
	// after being pulled from
	// main memory (RAM).
	vector<string> readLine;
	// Tracks whether the line
	// being written to was found
	// in the cache.
	bool foundLine = false;
	// Tracks whether the specific
	// set we are writing to is full.
	bool setFull = false;
	// The index of the line
	// being written to if it
	// is found in cache.
	int lineFoundIndex = 0;
	// The dirty bit to output,
	// will be 1 if the cache
	// and the RAM becomes
	// desynced through the
	// write action, or 0
	// otherwise.
	int dirtyBitToPrint = 0;

	// Extract the tag bits from the binary string.
	string tagBits = binaryAddress.substr(0, t);
	// Extract the set index bits from the binary string.
	string setIndex = binaryAddress.substr((0 + t), s);
	// Extract the block offset bits from the binary string.
	string blockOffSet = binaryAddress.substr((0 + t + s), b);

	// Convert the binary set index bits to a decimal index.
	int setIndexNumeric = BinaryToDecimal(setIndex);
	// Convert the block offset bits to an integer.
	int blockOffsetNumeric = BinaryToDecimal(blockOffSet);
	// Convert the tags bits to an integer.
	int tagBitsNumeric = BinaryToDecimal(tagBits);
	// Convert the entire memory address to an integer.
	int memoryAddress = BinaryToDecimal(binaryAddress);
	// Used to determine the address in RAM that we
	// will start pulling from if the byte we are
	// writing to needs to be moved from RAM to the cache.
	int adjustedMemAddress = ConvertToRAMAddress(binaryAddress);

	// Convert the tag bits to a hexadecimal format.
	temp << hex << tagBitsNumeric;
	hexTag = temp.str();
	// Make the hexadecimal uppercase.
	for (int i = 0; i < hexTag.length(); ++i) {
		hexTag.at(i) = toupper(hexTag.at(i));
	}

	// Print the set as a decimal,
	// and the tag as a hexadecimal.
	cout << "set:" << setIndexNumeric << endl;
	cout << "tag:" << hexTag << endl;

	// Determine the specific set we are working
	// with by using the integer version of the
	// set index bits. Note that fullCache's first
	// dimension is the sets, and the internal vector
	// stored within each set contains pairs of both
	// tag bits (to identify a line) and cache lines.
	std::vector<std::pair<string, CacheLine>>* set;
	set = &(fullCache.at(setIndexNumeric));

	// Check if we have the line we are
	// looking to write to in the cache.
	// If the line's valid bit is 1 and the
	// tag bits match, then we have found a match.
	for (int i = 0; i < set->size(); ++i) {
		if (set->at(i).first == tagBits && set->at(i).second.GetValidBit() == 1) {
			foundLine = true;
			lineFoundIndex = i;
			break;
		}
	}

	// Executes if the line we are writing to is not in the cache.
	if (!foundLine) {
		// Determine if the set is full, we can
		// do this by checking if the last line in
		// the set has a valid bit equal to 1.
		setFull = (set->at(set->size() - 1).second.GetValidBit() == 1);
		// Denote a cache miss.
		cout << "write_hit:no" << endl;
		// Increment the number of cache misses.
		++cacheMisses;

		// Executes if the set is not full.
		if (!setFull) {
			// Executes if the write-miss policy
			// is set to write-allocate.
			if (writeMissPolicy == 1) {
				// Reads the line from RAM that we
				// are looking to write to.
				readLine = RAM->ReadLine(adjustedMemAddress, B);
				// The index within the set that we
				// will be writing to, i.e. the next
				// available line.
				int indexToWrite = 0;
				// Create a pair object with the
				// cache line's tag bits and data
				// to store in the cache.
				pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
				// Set the source address of the cache
				// line in-case we are using a writeback
				// policy.
				currentLine.second.SetSourceAddress(adjustedMemAddress);

				// Determine the next open cache line
				// to write the data into.
				for (int i = 0; i < set->size(); ++i) {
					if (set->at(i).second.GetValidBit() == 0) {
						indexToWrite = i;
						// Write the newly created cache
						// line into the next open spot.
						set->at(i) = currentLine;
						break;
					}
				}

				// Denote which line was evicted.
				cout << "eviction_line:" << indexToWrite << endl;

				// Executes if the write-hit policy
				// is set to write-through.
				if (writeHitPolicy == 1) {
					// Write the data to the line in cache.
					set->at(indexToWrite).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					// Set the dirty bit to 0 because
					// we are writing-through.
					set->at(indexToWrite).second.SetDirtyBit(0);
					// Set the dirty bit to print to
					// 0 for the same reason.
					dirtyBitToPrint = 0;
					// Reset the timer on the cache line because
					// it was just used.
					set->at(indexToWrite).second.SetTimer();
					// Write through to the RAM.
					RAM->Write(memoryAddress, hexToStore);
				}
				// Executes if the write-hit policy
				// is set to write-back.
				else if (writeHitPolicy == 2) {
					// Write the data to the line in cache.
					set->at(indexToWrite).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					// Set the dirty bit to 1 because
					// the line will be desynced with
					// RAM.
					set->at(indexToWrite).second.SetDirtyBit(1);
					// Set the dirty bit to print to 1
					// for the same reason.
					dirtyBitToPrint = 1;
					// Reset the timer on the cache line
					// because it was just used.
					set->at(indexToWrite).second.SetTimer();
				}
			}
			// Executes if the write-miss policy is set to
			// no-write-allocate.
			else if (writeMissPolicy == 2) {
				// No line will be evicted when using
				// a no-write-allocate policy.
				cout << "eviction_line:-1" << endl;
				// Write the data only to RAM,
				// do not load it into cache.
				RAM->Write(memoryAddress, hexToStore);
				// Print 0 as the dirty bit because
				// there is no desync between RAM
				// and cache.
				dirtyBitToPrint = 0;
			}
		}
		// Executes if the set is full and
		// the replacement policy is set to
		// random replacement.
		else if (replacePolicy == 1 && setFull) {
			// Seed the RNG.
			srand(time(0));
			// Determine the max index for the given set.
			int maxIndex = (set->size());
			// Generate a random replacement index.
			int replacementIndex = rand() % maxIndex;
			
			// Executes if the write-miss policy is
			// set to write-allocate.
			if (writeMissPolicy == 1) {
				// Denote the randomly chosen eviction
				// line.
				cout << "eviction_line:" << replacementIndex << endl;
				// Read the line from RAM.
				readLine = RAM->ReadLine(adjustedMemAddress, B);
				// Create a line to be newly inserted into
				// the cache.
				pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
				// Set the source address of the
				// line in-case we are using write-back.
				currentLine.second.SetSourceAddress(adjustedMemAddress);
				// Write the evicted line back into RAM
				// if its dirty bit was set to 1.
				if (set->at(replacementIndex).second.GetDirtyBit() == 1) {
					RAM->WriteLine(set->at(replacementIndex).second.GetSourceAddress(), B, set->at(replacementIndex).second.GetCacheLine());
				}

				// Put the new replacement line into
				// the cache set.
				set->at(replacementIndex) = currentLine;

				// Executes if the write-hit policy is
				// set to write-through.
				if (writeHitPolicy == 1) {
					// Write to the line in cache.
					set->at(replacementIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					// Set the dirty bit to 0 because
					// it will be synced with RAM.
					set->at(replacementIndex).second.SetDirtyBit(0);
					// Set the dirty bit to print to 0
					// for the same reason.
					dirtyBitToPrint = 0;
					// Reset the timer for the line because it
					// was just used.
					set->at(replacementIndex).second.SetTimer();

					// Write the same data to RAM because
					// we are using a write-through policy.
					RAM->Write(memoryAddress, hexToStore);
				}
				// Executes if we are using a write-back policy.
				else if (writeHitPolicy == 2) {
					// Write to the line in the cache.
					set->at(replacementIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					// Set the dirty bit to 1 because the
					// line in cache will be desynced with
					// RAM.
					set->at(replacementIndex).second.SetDirtyBit(1);
					// Set the dirty bit to print to 1
					// for the same reason.
					dirtyBitToPrint = 1;
					// Reset the line's timer because it was
					// just used.
					set->at(replacementIndex).second.SetTimer();
				}
			}
			// Executes if the write-miss policy
			// is set to no-write allocate.
			else if (writeMissPolicy == 2) {
				// No line will be evicted when using
				// a no-write-allocate policy.
				cout << "eviction_line:-1" << endl;
				// Write directly to the RAM, ignoring
				// the cache.
				RAM->Write(memoryAddress, hexToStore);
				// Set the dirty bit to print to 0 because
				// there is no desync with RAM.
				dirtyBitToPrint = 0;
			}
		}
		// Executes if the set is full and
		// the replacement policy is set
		// to least-recently-used.
		else if (replacePolicy == 2 && setFull) {
			// Will keep the index of the line
			// used the furthest in the past.
			int leastIndex = -1;
			// Set the least time to be equal to
			// the time of the first line in the set.
			std::chrono::steady_clock::time_point leastTime = set->at(0).second.GetTime();
			// Set the index of the line used the furthest
			// in the past to the first cache line.
			leastIndex = 0;

			// Loop through all cache lines and determine
			// which line was used the furthest in the
			// past, updating leastTime and leastIndex
			// accordingly.
			for (int i = 1; i < set->size(); ++i) {
				if (set->at(i).second.GetTime() < leastTime) {
					leastTime = set->at(i).second.GetTime();
					leastIndex = i;
				}
			}

			// Executes if the write-miss
			// policy is set to write-allocate.
			if (writeMissPolicy == 1) {
				// Denote the chosen eviction line.
				cout << "eviction_line:" << leastIndex << endl;
				// Read the line to write to from
				// RAM.
				readLine = RAM->ReadLine(adjustedMemAddress, B);
				// Create a new cache line with the data
				// read from RAM.
				pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
				// Set the source address of the cache
				// line in-case we are using write-back.
				currentLine.second.SetSourceAddress(adjustedMemAddress);
				// Write the evicted line back to RAM if its
				// dirty bit was set to 1.
				if (set->at(leastIndex).second.GetDirtyBit() == 1) {
					RAM->WriteLine(set->at(leastIndex).second.GetSourceAddress(), B, set->at(leastIndex).second.GetCacheLine());
				}
				// Write the new line into cache.
				set->at(leastIndex) = currentLine;

				// Executes if the write-hit
				// policy is set to write-through.
				if (writeHitPolicy == 1) {
					// Write the data to the new cahe line.
					set->at(leastIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					// Set the dirty bit to 0 because the cache
					// will not be desynced from RAM.
					set->at(leastIndex).second.SetDirtyBit(0);
					// Set the dirty bit to 0 because the cache
					// and RAM will be synced.
					dirtyBitToPrint = 0;
					// Reset the line's timer because it was just used.
					set->at(leastIndex).second.SetTimer();

					// Write through to the RAM.
					RAM->Write(memoryAddress, hexToStore);
				}
				// Executes if the write-hit policy
				// is set to write-back.
				else if (writeHitPolicy == 2) {
					// Write the data to the new cache line.
					set->at(leastIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					// Set the dirty bit to 1 because it will
					// be desynced from RAM.
					set->at(leastIndex).second.SetDirtyBit(1);
					// Set the dirty bit to print to 1 for
					// the same reason.
					dirtyBitToPrint = 1;
					// Reset the line's timer because it was
					// just used.
					set->at(leastIndex).second.SetTimer();
				}
			}
			// Executes if the write-miss policy
			// is set to no-write-allocate.
			else if (writeMissPolicy == 2) {
				// No line will be evicted when using
				// a no-write-allocate policy.
				cout << "eviction_line:-1" << endl;
				// Write directly to the RAM, ignoring
				// the cache.
				RAM->Write(memoryAddress, hexToStore);
				// Set the dirty bit to print to 0
				// because the RAM and the cache will
				// be synced.
				dirtyBitToPrint = 0;
			}
		}

		// Print the RAM address that was either
		// written to, or pulled into cache and then
		// written to.
		cout << "ram_address:0x" << hexAddressToPrint << endl;
		// Print the data that was written.
		cout << "data:0x" << hexToStore << endl;
		// Print the state of the dirty bit.
		cout << "dirty_bit:" << dirtyBitToPrint << endl;
	}
	// Executes if we have a write-hit.
	else {
		// Denotes a write-hit.
		cout << "write_hit:yes" << endl;
		// Denote that no line was evicted.
		cout << "eviction_line:-1" << endl;
		// Denote that no RAM needed to be read.
		cout << "ram_address:-1" << endl;
		// Print the data that was written.
		cout << "data:0x" << hexToStore << endl;
		// Increment the number of cache hits.
		++cacheHits;

		// Executes if the write-hit policy is
		// set to write-through.
		if (writeHitPolicy == 1) {
			// Write the data to the found line in cache.
			set->at(lineFoundIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
			// Set the dirty bit to 0 because the
			// cache and RAM will be synced.
			set->at(lineFoundIndex).second.SetDirtyBit(0);
			// Set the dirty bit to print to 0
			// for the same reason.
			dirtyBitToPrint = 0;
			// Set the timer on the cache line
			// because it was just used.
			set->at(lineFoundIndex).second.SetTimer();

			// Write the data through to the RAM.
			RAM->Write(memoryAddress, hexToStore);
		}
		// Executes if the write-hit policy is
		// set to write-back.
		else if (writeHitPolicy == 2) {
			// Write the data to the found line in cache.
			set->at(lineFoundIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
			// Set the dirty bit to 1 because the
			// cache and RAM will not be synced.
			set->at(lineFoundIndex).second.SetDirtyBit(1);
			// Set the dirty bit to print to 1
			// for the same reason.
			dirtyBitToPrint = 1;
			// Reset the timer for the line since it
			// was just used.
			set->at(lineFoundIndex).second.SetTimer();
		}

		// Print the state of the dirty bit.
		cout << "dirty_bit:" << dirtyBitToPrint << endl;
	}
}

// Clears the entirety of the cache
// by resetting the data, resetting
// the valid and dirty bits, and
// syncing the RAM if any lines
// in the cache has a dirty bit
// asserted.
void Cache::CacheFlush()
{
	// Print out for the user that
	// the cache has been cleared.
	cout << "cache_cleared" << endl;

	// Go through each set,
	// and then internally through
	// each line in the set and 
	// sync the line with RAM if
	// the dirty bit was asserted,
	// and set all data, the valid
	// bit, and dirty bit to 0s.
	for (int i = 0; i < fullCache.size(); ++i) {
		for (int j = 0; j < fullCache.at(i).size(); ++j) {
			if (fullCache.at(i).at(j).second.GetDirtyBit() == 1) {
				RAM->WriteLine(fullCache.at(i).at(j).second.GetSourceAddress(), B, fullCache.at(i).at(j).second.GetCacheLine());
			}
		}
		// Resets the cache line.
		fullCache.at(i) = vector<pair<string, CacheLine>>(E, pair<string, CacheLine>("", (B)));
	}
}

// Dumps all data from the cache
// into a file named "cache.txt"
// This function will overwrite
// the file each time, not append
// to it.
void Cache::CacheDump()
{
	// Create the output file
	// stream.
	ofstream outFS;
	// Open cache.txt, this
	// will either open an
	// existing file or create
	// a new one if the file
	// does not already exist.
	outFS.open("cache.txt");
	
	// Dynamically keeps track of the set
	// we are working with.
	std::vector<std::pair<std::string, CacheLine>>* set;
	// Iterate through each set, and
	// then each line within each set
	// to print the data from that line
	// into the file.
	for (int i = 0; i < fullCache.size(); ++i) {
		set = &(fullCache.at(i));
		for (int j = 0; j < set->size(); ++j) {
			set->at(j).second.PrintCacheLineToFile(outFS);
			if (j == (set->size() - 1) && i == (fullCache.size() - 1)) 
			{
				// Don't print a space.
			}
			else {
				outFS << endl;
			}
		}
	}

	// Close the output file.
	outFS.close();
}

// Will print all the details of the 
// cache's configuration, as well as the
// current contents of the cache.
void Cache::PrintCacheContents()
{
	// Print the cache size.
	cout << "cache_size:" << C << endl;
	// Print the size of the data blocks in the cache.
	cout << "data_block_size:" << B << endl;
	// Print the number of lines per set.
	cout << "associativity:" << E << endl;

	// Print the replacement policy
	// used by the cache.
	cout << "replacement_policy:";
	if (replacePolicy == 1) {
		cout << "random_replacement" << endl;
	}
	else if (replacePolicy == 2) {
		cout << "least_recently_used" << endl;
	}

	// Print the write-hit policy used
	// by the cache.
	cout << "write_hit_policy:";
	if (writeHitPolicy == 1) {
		cout << "write_through" << endl;
	}
	else if (writeHitPolicy == 2) {
		cout << "write_back" << endl;
	}

	// Print the write-miss policy used
	// by the cache.
	cout << "write_miss_policy:";
	if (writeMissPolicy == 1) {
		cout << "write_allocate" << endl;
	}
	else if (writeMissPolicy == 2) {
		cout << "no_write_allocate" << endl;
	}

	// Print the number of cache hits
	// and misses. Includes both hits
	// and misses for cache reads &
	// writes.
	cout << "number_of_cache_hits:" << cacheHits << endl;
	cout << "number_of_cache_misses:" << cacheMisses << endl;

	// Print all cache content, including
	// the valid bits, dirty bits, tag bits,
	// and data of each line.
	cout << "cache_content:" << endl;
	std::vector<std::pair<std::string, CacheLine>>* set;
	for (int i = 0; i < fullCache.size(); ++i) {
		set = &(fullCache.at(i));
		for (int j = 0; j < set->size(); ++j) {
			// Prints the valid bit.
			cout << set->at(j).second.GetValidBit() << " ";
			// Prints the dirty bit.
			cout << set->at(j).second.GetDirtyBit() << " ";
			// Prints the hexadecimal tag.
			cout << setw(2) << setfill('0') << BinaryToHex(set->at(j).first) << " ";
			// Print the data in the line.
			set->at(j).second.PrintCacheLine();
			cout << endl;
		}
	}
}

// An internal helper function that converts
// a string of binary digits into a decimal
// numeric.
int Cache::BinaryToDecimal(std::string binaryNumber)
{
	// The converted decimal
	// number.
	int decimalNum = 0;
	// Used for the conversion process.
	int multiplier = 0;
	
	// Standard method of converting
	// from binary to decimal; i.e.
	// base_2 to base_10.
	for (int i = (binaryNumber.length() - 1); i >= 0 ; --i) {
		if (binaryNumber.at(i) == '0') {
			++multiplier;
		}
		else if (binaryNumber.at(i) == '1') {
			decimalNum += (1 * pow(2, multiplier));
			++multiplier;
		}
	}

	// Return the converted
	// number.
	return decimalNum;
}

// An internal helper function that
// converts a string of binary digits
// to a string hexadecimal address
// in all caps.
std::string Cache::BinaryToHex(std::string binaryNumber)
{
	ostringstream outSS;
	// The hexadecimal string to
	// return.
	string hexToPrint = "";
	// Decimal number that will be
	// used as an intermediary.
	int decimalNumber = 0;

	// Convert the string of binary
	// digits into a decimal numeric.
	decimalNumber = BinaryToDecimal(binaryNumber);
	// Convert the decimal numeric
	// into a hexidecimal and send
	// it to the output string stream.
	outSS << hex << decimalNumber;
	// Convert the output string stream
	// into a standard string.
	hexToPrint = outSS.str();
	// Make the converted string uppercase.
	MakeStringUppercase(hexToPrint);

	// Return the converted string.
	return hexToPrint;
}

// An internal helper function
// that converts a string into
// all uppercase character.
void Cache::MakeStringUppercase(std::string& stringToChange)
{
	// Iterate through each letter in
	// the string and make it uppercase.
	for (int i = 0; i < stringToChange.length(); ++i) {
		stringToChange.at(i) = toupper(stringToChange.at(i));
	}
}

// An internal helper function that
// converts a binary address for a specific
// byte into its associated RAM address. Note
// that when querying the RAM for information,
// bytes around the one we are looking for are
// also pulled into the cache to take advantage
// of spatial locality; this function allows us
// to determine the alternative bytes we will be
// pulling in.
int Cache::ConvertToRAMAddress(std::string addressString)
{
	// The number of offset bits.
	int offset = 0;
	// Will be used to determine how
	// many offset bits we are setting to 0.
	offset = b;

	// Set all block offset bits to 0
	// in order to determine our respective
	// RAM address associated with the passed
	// in address.
	for (int i = addressString.length() - 1; i >= 0 && offset > 0; --i) {
		addressString.at(i) = '0';
		--offset;
	}

	// Convert the string RAM address into
	// a decimal index.
	int ramAddress = 0;
	ramAddress = BinaryToDecimal(addressString);

	// Return the RAM index.
	return ramAddress;
}
