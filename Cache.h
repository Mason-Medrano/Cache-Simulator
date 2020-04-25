#ifndef _CACHE_H_
#define _CACHE_H_

#include <vector>
#include <utility>
#include <string>
#include "CacheLine.h"
#include "MainMemory.h"

class Cache {
public:
	Cache();
	Cache(int cacheSize, int blockSize, int linesPerSet, int replacePolicyInput, int writeHPInput, int writeMPInput, MainMemory* ptrToRAM);
	std::string CacheRead(std::string binaryAddress, std::string hexAddressToPrint);
	void CacheWrite(std::string binaryAddress, std::string hexToStore, std::string hexAddressToPrint);
	void CacheFlush();
	void CacheDump();
	void PrintCacheContents();

private:
	int C;
	int E;
	int S;
	int B;
	int s;
	int b;
	int t;
	int replacePolicy;
	int writeHitPolicy;
	int writeMissPolicy;
	int cacheHits;
	int cacheMisses;
	std::vector<std::vector<std::pair<std::string, CacheLine>>> fullCache;
	MainMemory* RAM;

	int BinaryToDecimal(std::string binaryNumber);
	std::string BinaryToHex(std::string binaryNumber);
	void MakeStringUppercase(std::string& stringToChange);
	int ConvertToRAMAddress(std::string addressString);
};
#endif // !_CACHE_H_

