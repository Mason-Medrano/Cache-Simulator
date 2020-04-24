#include <fstream>
#include <iostream>
#include <sstream>
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
	int addressOfLine = 0;

	if (sizeOfLine < 8) {
		addressOfLine = ramAddress;
	}
	else if (sizeOfLine == 8) {
		int deduction = 0;
		deduction = ramAddress % 8;

		addressOfLine = ramAddress - deduction;
	}
	else {
		addressOfLine = ramAddress;
	}

	vector<string> fullLine;

	for (int i = addressOfLine; i < (addressOfLine + sizeOfLine); ++i) {
		fullLine.push_back(bytes.at(i));
	}

	return fullLine;
}

void MainMemory::WriteLine(int ramAddress, int sizeOfLine, std::vector<std::string> lineToWrite)
{
	int addressOfLine = 0;
	int writeIndex = 0;

	if (sizeOfLine < 8) {
		addressOfLine = ramAddress;
	}
	else if (sizeOfLine == 8) {
		int deduction = 0;
		deduction = ramAddress % 8;

		addressOfLine = ramAddress - deduction;
	}
	else {
		addressOfLine = ramAddress;
	}

	for (int i = addressOfLine; i < (addressOfLine + sizeOfLine); ++i) {
		bytes.at(i) = lineToWrite.at(writeIndex);
		++writeIndex;
	}
}

void MainMemory::Write(int ramAddress, string data)
{
	bytes.at(ramAddress) = data;
}

int MainMemory::GetMemorySize()
{
	return bytes.size();
}

void MainMemory::PrintMemoryContents()
{
	ostringstream outSS;
	string convertedAddress = "";

	for (int i = 0; i < bytes.size(); i += 8) {
		outSS << hex << i;
		convertedAddress = outSS.str();
		MakeStringUppercase(convertedAddress);
		cout << "0x" << convertedAddress << ":";
		for (int j = i; j < (i + 8); ++j) {
			cout << bytes.at(j) << " ";
		}
		cout << dec << endl;
		outSS.str("");
	}
}

void MainMemory::MemoryDump()
{
	ofstream outFS;

	outFS.open("ram.txt");

	for (int i = 0; i < bytes.size(); ++i) {
		if (i == (bytes.size() - 1)) {
			outFS << bytes.at(i);
		}
		else {
			outFS << bytes.at(i) << endl;
		}
	}

	outFS.close();
}

void MainMemory::MakeStringUppercase(std::string& stringToChange)
{
	for (int i = 0; i < stringToChange.length(); ++i) {
		stringToChange.at(i) = toupper(stringToChange.at(i));
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