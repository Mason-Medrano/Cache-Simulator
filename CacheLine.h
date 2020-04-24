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
	void WriteToCacheLine(int blockOffset, std::string dataToWrite);
	std::vector<std::string> GetCacheLine();
	void SetData(std::vector<std::string> dataToSet);
	void PrintCacheLine();
	void PrintCacheLineToFile(std::ofstream& ofs);
	void AssertValidBit();
	void SetDirtyBit(int bit);
	void SetTimer();
	void SetSourceAddress(int sourceAddressInput);
	int GetSourceAddress();
	int GetValidBit();
	int GetDirtyBit();
	std::chrono::steady_clock::time_point GetTime();
private:
	int validBit;
	int dirtyBit;
	std::chrono::steady_clock::time_point timeInCache;
	std::vector<std::string> data;
	int sourceAddress;
};

#endif // !_LINE_H_
