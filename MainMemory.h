#ifndef _MAIN_MEMORY_H_
#define _MAIN_MEMORY_H_

#include <string>
#include <vector>

class MainMemory {
public:
	MainMemory();
	MainMemory(std::string inputFile);
	std::string Read(int ramAddress);
	std::vector<std::string> ReadLine(int ramAddress, int sizeOfLine);
	void WriteLine(int ramAddress, int sizeOfLine, std::vector<std::string> lineToWrite);
	void Write(int ramAddress, std::string data);
	int GetMemorySize();
	void PrintMemoryContents();
	void MemoryDump();
	void MakeStringUppercase(std::string& stringToChange);
private:
	std::vector<std::string> bytes;

	void PopulateMemory(std::string memoryContentsFile);
};

#endif // !_MAIN_MEMORY_H_

