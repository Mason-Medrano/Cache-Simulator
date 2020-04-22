#ifndef _CACHE_LINE_H_
#define _CACHE_LINE_H_

#include <string>
#include <vector>
#include <chrono>
#include <fstream>

class CacheLine {
public:
	CacheLine();
	CacheLine(int blockSize);
	CacheLine(std::vector<std::string> dataToSet);
	std::string ReadFromCacheLine(int blockOffset);
	void SetData(std::vector<std::string> dataToSet);
	void PrintCacheLine();
	void PrintCacheLineToFile(std::ofstream& ofs);
	void AssertValidBit();
	void FlipDirtyBit();
	void SetTimer();
	int GetValidBit();
	int GetDirtyBit();
	std::chrono::steady_clock::time_point GetTime();
private:
	int validBit;
	int dirtyBit;
	std::chrono::steady_clock::time_point timeInCache;
	std::vector<std::string> data;
};

#endif // !_LINE_H_
