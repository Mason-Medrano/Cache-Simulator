#include <fstream>
#include <iostream>
#include "MainMemory.h"
using namespace std;

MainMemory::MainMemory()
	: bytes(256)
{

}

MainMemory::MainMemory(std::string inputFile)
	: bytes(256)
{
	try {
		PopulateMemory(inputFile);
	}
	catch (invalid_argument & invalidArgExcpt) {
		cerr << invalidArgExcpt.what() << endl;
	}
}

std::string MainMemory::Read(int ramAddress)
{
	return bytes.at(ramAddress);
}

std::vector<string> MainMemory::ReadLine(int ramAddress, int sizeOfLine)
{
	vector<string> fullLine;

	for (int i = ramAddress; i < (ramAddress + sizeOfLine); ++i) {
		fullLine.push_back(bytes.at(i));
	}

	return fullLine;
}

void MainMemory::Write(std::string data, int ramAddress)
{
	bytes.at(ramAddress) = data;
}

int MainMemory::GetMemorySize()
{
	return bytes.size();
}

void MainMemory::PrintMemoryContents()
{
	for (int i = 0; i < bytes.size(); i += 8) {
		cout << "0x" << hex << i << ":";
		for (int j = i; j < (i + 8); ++j) {
			cout << bytes.at(j) << " ";
		}
		cout << dec << endl;
	}
}

void MainMemory::PopulateMemory(std::string memoryContentsFile)
{
	cout << "initialize the RAM:" << endl;
	cout << "init-ram 0x00 0xFF" << endl;
	
	ifstream inFS;
	string hexData = "";
	int memoryIndex = 0;
	
	inFS.open(memoryContentsFile);

	if (!inFS.is_open()) {
		throw invalid_argument("Invalid Argument: The specified input file cannot be opened.");
	}

	while (!inFS.eof()) {
		getline(inFS, hexData);
		bytes.at(memoryIndex) = hexData;
		++memoryIndex;
	}

	cout << "ram successfully initialized!" << endl;
}