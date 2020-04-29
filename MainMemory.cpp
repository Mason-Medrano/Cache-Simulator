// File: MainMemory.cpp
// Author(s): Shane McGookey & Mason Medrano
// Date: 04/22/2020
// Section: CSCE 312-511 (Both)
// E-mail: Shane_McGookey@tamu.edu & masonmedrano@tamu.edu
// Description: This file includes the function definitions
//              for the MainMemory class. These definitions
//              include various constructors, as well as 
//              functions that allow the caller to read from
//              and write to the Main Memory (RAM). There is
//              also a function that populates Main Memory with
//              the contents of a specified input file.

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "MainMemory.h"
using namespace std;

// The default constructor for the
// MainMemory class. Constructs a
// Main Memory capable of holding
// 256 bytes of data.
MainMemory::MainMemory()
	: bytes(256)
{
	for (int i = 0; i < bytes.size(); ++i) {
		bytes.at(i) = "00";
	}
}

// The custom constructor for the
// MainMemory class. This constructor
// takes a file as its argument, and will
// populate Main Memory with the contents of
// the specified file.
MainMemory::MainMemory(std::string inputFile)
	: bytes(256)
{
	for (int i = 0; i < bytes.size(); ++i) {
		bytes.at(i) = "00";
	}
	
	// Will attempt to populate the
	// Main Memory with the contents
	// of the specified input file, and
	// will handle errors in the event that
	// there is a problem during placing the
	// data into memory.
	try {
		// Makes a call to the internal
		// PopulateMemory() helper function.
		PopulateMemory(inputFile);
	}
	catch (invalid_argument & invalidArgExcpt) {
		cerr << invalidArgExcpt.what() << endl;
	}
}

// Returns the byte of data
// at a specified address in memory.
std::string MainMemory::Read(int ramAddress)
{
	return bytes.at(ramAddress);
}

// Reads an entire line of length
// 'sizeOfLine' from Main Memory.
// The read begins from the 'ramAddress'
// passed-in as an argument.
std::vector<string> MainMemory::ReadLine(int ramAddress, int sizeOfLine)
{
	// Create a vector to hold the line
	// of data being read.
	vector<string> fullLine;

	// Populate the vector.
	for (int i = ramAddress; i < (ramAddress + sizeOfLine); ++i) {
		fullLine.push_back(bytes.at(i));
	}

	// Return the line read from
	// Main Memory.
	return fullLine;
}

// Writes an entire line of
// length 'sizeOfLine' to Main Memory.
// The write begins from the 'ramAddress'
// passed-in as an argument.
void MainMemory::WriteLine(int ramAddress, int sizeOfLine, std::vector<std::string> lineToWrite)
{
	// Keeps our index for writing data
	// relative to the starting address.
	int writeIndex = 0;

	// Write process
	for (int i = ramAddress; i < (ramAddress + sizeOfLine); ++i) {
		bytes.at(i) = lineToWrite.at(writeIndex);
		++writeIndex;
	}
}

// Write to a specific byte of data
// at a specified address in memory.
void MainMemory::Write(int ramAddress, string data)
{
	bytes.at(ramAddress) = data;
}

// Returns the total size of
// the memory in bytes.
int MainMemory::GetMemorySize()
{
	return bytes.size();
}

// Prints the size of the RAM,
// as well as all of the data
// contained in RAM. The function
// prints a formatted 8-bytes per
// line of RAM data.
void MainMemory::PrintMemoryContents()
{
	// Print the total size in bytes of the main memory.
	cout << "memory_size:" << GetMemorySize() << endl;
	// Print the memory's contents.
	cout << "memory_content:" << endl;
	cout << "Address:Data" << endl;
	
	ostringstream outSS;
	// Will hold the hexadecimal address to print.
	string convertedAddress = "";

	// Print the data contained in RAM.
	for (int i = 0; i < bytes.size(); i += 8) {
		// Convert the index to hexadecimal,
		// and push it to an output string
		// stream.
		outSS << hex << i;
		// Move the contents of the output
		// string stream into a string.
		convertedAddress = outSS.str();
		// Convert all the hexadecimal digits
		// to uppercase to match required output.
		MakeStringUppercase(convertedAddress);
		// Output the address in hexadecimal
		// with leading zeroes (if necessary).
		cout << "0x" << setw(2) << setfill('0') << convertedAddress << ":";
		// Print the 8 bytes of data that begin at
		// the starting address just printed.
		for (int j = i; j < (i + 8); ++j) {
			cout << bytes.at(j) << " ";
		}

		// Change the state of the output stream back.
		cout << dec << endl;

		// Refresh the output string stream.
		outSS.str("");
	}
}

// Dumps the contents of the Main Memory
// to a file called "ram.txt". 
void MainMemory::MemoryDump()
{
	ofstream outFS;

	// Open the file ram.txt.
	// Will either update an existing
	// file, or will create a new one.
	outFS.open("ram.txt");

	// Write the data currently stored
	// in RAM to the output file, do not
	// include a newline for the last byte
	// of data.
	for (int i = 0; i < bytes.size(); ++i) {
		if (i == (bytes.size() - 1)) {
			outFS << bytes.at(i);
		}
		else {
			outFS << bytes.at(i) << endl;
		}
	}

	// Close the output file.
	outFS.close();
}

// A helper function that makes
// a specified string uppercase.
// This is used to print uppercase
// hexadecimal addresses.
void MainMemory::MakeStringUppercase(std::string& stringToChange)
{
	// Change each letter in the string to uppercase.
	for (int i = 0; i < stringToChange.length(); ++i) {
		stringToChange.at(i) = toupper(stringToChange.at(i));
	}
}

// Populates the Main Memory with the
// data in the specified input file
// 'memoryContentsFile'. 
void MainMemory::PopulateMemory(std::string memoryContentsFile)
{
	// Beginning initialization statements.
	cout << "initialize the RAM:" << endl;
	cout << "init-ram 0x00 0xFF" << endl;
	
	ifstream inFS;
	string hexData = "";
	int memoryIndex = 0;
	bool errorFlag = false;
	
	try {
		// Open the file to read the contents
		// of memory from.
		inFS.open(memoryContentsFile);

		// Throw an exception in the event that the specified
		// file could not be opened.
		if (!inFS.is_open()) {
			throw invalid_argument("Invalid Argument: The specified input file cannot be opened.");
		}
	}
	catch (invalid_argument & invalidArgExcpt) {
		cerr << invalidArgExcpt.what() << endl;
		errorFlag = true;
	}

	if (inFS.is_open()) {
		// Continue to read data from the input
		// file into Main Memory.
		while (!inFS.eof()) {

			// Get the next line in the file.
			getline(inFS, hexData);
			
			// Ignore whitespace.
			if (hexData == "") {
				continue;
			}

			try {
				// Check for valid hexadecimal data.
				if (hexData.length() > 2) {
					throw runtime_error("Error: Input in input file is not valid hexadecimal RAM data. Please check your input file.");
				}

				string tempHex = "";

				// Second check for valid hexadecimal data.
				for (int i = 0; i < hexData.length(); ++i) {
					tempHex = tolower(hexData.at(i));
				}

				// Second check for valid hexadecimal data.
				for (int j = 0; j < tempHex.length(); ++j) {
					if (!(isalpha(tempHex.at(j)) || isdigit(tempHex.at(j))) || tempHex.at(j) > 'f') {
						throw invalid_argument("Invalid hexadecimal data: 0x" + hexData);
					}
				}
			}
			catch (invalid_argument & invalidArgExcpt) {
				cerr << invalidArgExcpt.what() << endl;
				errorFlag = true;
			}
			
			// Make sure that the input file does
			// not contain more data than the RAM
			// is meant to hold.
			try {
				if (memoryIndex == bytes.size()) {
					throw runtime_error("Error: Data file is too large. RAM can only contain 256 bytes of data (0x00 - 0xFF).");
				}
			}
			// Throw an exception if the input
			// file is too large; will still use
			// the data populated up to this point.
			catch (runtime_error & rtError) {
				cerr << rtError.what() << endl;
				errorFlag = true;
				break;
			}
			
			// Write that data into Main Memory.
			bytes.at(memoryIndex) = hexData;
			// Increment the index to store data at.
			++memoryIndex;
		}

		// Check to see if the input file was empty,
		// and throw an error if so since the RAM
		// will not contain any inputted content.
		if (memoryIndex == 0) {
			cerr << "Error: Input file was empty and therefore RAM contains no data." << endl;
			errorFlag = true;
		}
	}

	// Ending statements.
	if (errorFlag) {
		cout << "ram initialization failed!" << endl;
		if (inFS.is_open()) {
			inFS.close();
		}
		exit(1);
	}
	else {
		cout << "ram successfully initialized!" << endl;
	}

	// Close the input file.
	inFS.close();
}