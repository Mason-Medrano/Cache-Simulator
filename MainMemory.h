#ifndef _MAIN_MEMORY_H_
#define _MAIN_MEMORY_H_

#include <string>
#include <vector>

class MainMemory {
public:
	MainMemory();
	MainMemory(std::string inputFile);
	std::string Read(int ramAddress);
	std::vector<string> ReadLine(int ramAddress, int sizeOfLine);
	void Write(std::string data, int ramAddress);
	int GetMemorySize();
	void PrintMemoryContents();
private:
	std::vector<std::string> bytes;

	void PopulateMemory(std::string memoryContentsFile);
};

#endif // !_MAIN_MEMORY_H_

