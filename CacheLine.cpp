// File: CacheLine.cpp
// Author(s): Shane McGookey & Mason Medrano
// Date: 04/22/2020
// Section: CSCE 312-511 (Both)
// E-mail: Shane_McGookey@tamu.edu & masonmedrano@tamu.edu
// Description: This file includes the function definitions
//              for the CacheLine class. The CacheLine class
//              represents a single line stored within a set,
//              and can be read from, written to, printed, etc.
//              The cache line also stores what address it should
//              be returned to if evicted under a write-back policy
//              and stores its dirty bit and valid bit.

#include <iostream>
#include "CacheLine.h"
using namespace std;

// The default constructor for a CacheLine,
// sets the validBit and dirtyBit to 0, creates
// a vector of size 0 to store the line's data,
// and sets the source address to -1.
CacheLine::CacheLine()
	: validBit(0), dirtyBit(0), data(0), sourceAddress(-1)
{
	// All operations are completed in the
	// constructor initializer list.
}

// The custom constructor for a CacheLine;
// takes the size of the data block stored
// within the line, and creates a line of that
// size that stores all '00's. The rest of the
// construction is the same as it is in the
// default constructor.
CacheLine::CacheLine(int blockSize)
	: validBit(0), dirtyBit(0), data(blockSize, "00"), sourceAddress(-1)
{
	// All operations are completed in the
	// constructor initializer list.
}

// The custom constructor for a CacheLine;
// takes a vector of data as an argument
// that it will construct the CacheLine with.
// The valid bit is set to 1 because this function
// is only called when populating the line with new
// valid data, and the rest of the data members are
// set to 0.
CacheLine::CacheLine(std::vector<std::string> dataToSet)
	: validBit(1), dirtyBit(0), sourceAddress(0)
{
	// Call the internal helper function
	// SetData().
	SetData(dataToSet);
}

// Reads a byte from the cache line
// as per a specified block offset
// (reading from left to right).
std::string CacheLine::ReadFromCacheLine(int blockOffset)
{
	// blockOffset is used as an index
	// to address the vector storing the
	// data, where the first byte is at index 0,
	// the second at index 1, etc.
	return data.at(blockOffset);
}

// Writes a byte (dataToWrite) to the 
// cache line as per a specified block
// offset (going from left to right).
void CacheLine::WriteToCacheLine(int blockOffset, string dataToWrite)
{
	// blockOffset is used as an index
	// to address the vector storing the
	// data, where the first byte is at index 0,
	// the second at index 1, etc.
	data.at(blockOffset) = dataToWrite;
}

// Returns the entire data contents
// of the cache line (not the dirty
// bit or valid bit).
std::vector<std::string> CacheLine::GetCacheLine()
{
	// Return the cache line's data.
	return data;
}

// Sets the entire data contents
// of the cache line (not the dirty
// bit or valid bit).
void CacheLine::SetData(std::vector<std::string> dataToSet)
{
	data = dataToSet;
}

// Prints all of the data stored
// within the cache line.
void CacheLine::PrintCacheLine()
{
	// Iterate through each byte of data
	// and print that data.
	for (int i = 0; i < data.size(); ++i) {
		cout << data.at(i) << " ";
	}
}

// Prints all of the data stored
// within the cache line to a file.
void CacheLine::PrintCacheLineToFile(ofstream& ofs)
{
	// Iterate through each byte of data
	// and print that data to the file
	// specified in the function's argument.
	for (int i = 0; i < data.size(); ++i) {
		ofs << data.at(i) << " ";
	}
}

// Asserts the valid bit to
// specify that the cache line
// contains valid data.
void CacheLine::AssertValidBit()
{
	this->validBit = 1;
}

// Sets the dirty bit equal
// to the 'bit' passed-in to
// the function.
void CacheLine::SetDirtyBit(int bit)
{
	dirtyBit = bit;
}

// Sets the dynamic timer for the
// cache line to the current time
// when the function is called.
void CacheLine::SetTimer()
{
	// Set timeInCache to the current time when
	// the function is called.
	timeInCache = chrono::steady_clock::now();
}

// Sets the source address of the line,
// this is where the line should be returned
// to in the RAM if the write-back policy
// is being used.
void CacheLine::SetSourceAddress(int sourceAddressInput)
{
	sourceAddress = sourceAddressInput;
}

// Returns the source address, i.e. the
// address in RAM the line should be returned
// to.
int CacheLine::GetSourceAddress()
{
	return sourceAddress;
}

// Returns the valid bit.
int CacheLine::GetValidBit()
{
	return validBit;
}

// Returns the dirty bit.
int CacheLine::GetDirtyBit()
{
	return dirtyBit;
}

// Returns the time of last use
// associated with the line.
std::chrono::steady_clock::time_point CacheLine::GetTime()
{
	return timeInCache;
}
