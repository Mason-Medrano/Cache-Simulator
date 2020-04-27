// File: MainMemory.h
// Author(s): Shane McGookey & Mason Medrano
// Date: 04/22/2020
// Section: CSCE 312-511 (Both)
// E-mail: Shane_McGookey@tamu.edu & masonmedrano@tamu.edu
// Description:
// This is the header file containing class delcaration for RAM
#ifndef _MAIN_MEMORY_H_
#define _MAIN_MEMORY_H_

#include <string>
#include <vector>

class MainMemory {
public:
	/*Default and parameterized constructor for RAM.
	  RAM is populated accordingly.*/
	MainMemory(); 
	MainMemory(std::string inputFile); 

	/*Returns a string of data from the specified address in RAM*/
	std::string Read(int ramAddress); 

	/*Returns a vector of strings representing data from RAM.
	  The size of the vector is dependent on the size of a cache line.*/
	std::vector<std::string> ReadLine(int ramAddress, int sizeOfLine);

	/*Writes a vector of strings to RAM at a specified address,
	  with the size of the vector dependen on the size of a cache line.*/
	void WriteLine(int ramAddress, int sizeOfLine, std::vector<std::string> lineToWrite);

	/*Writes a string of data to the specified address in RAM.*/
	void Write(int ramAddress, std::string data);

	/*Returns a an integer value representing the size of memory.*/
	int GetMemorySize();

	/*Outputs to the console the entire contents of RAM.*/
	void PrintMemoryContents();

	/*Outputs the entire contents of RAM to a ram.txt file.*/
	void MemoryDump();

	/*Helper function which capitalizes all letters in a given string.*/
	void MakeStringUppercase(std::string& stringToChange);
private:

	/*Vector containing data in RAM of type string.*/
	std::vector<std::string> bytes;

	/*Helper function which reads data from the input file
	  and places it in bytes vector. This function is used
	  in the parameterized constructor.*/
	void PopulateMemory(std::string memoryContentsFile);
};

#endif // !_MAIN_MEMORY_H_

