#ifndef _CACHE_H_
#define _CACHE_H_

#include <vector>
#include <map>
#include <string>
#include "CacheLine.h"

class Cache {
public:
	Cache();
	Cache(int cacheSize, int blockSize, int linesPerSet, int replacePolicyInput, int writeHPInput, int writeMPInput);


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
	std::vector<std::map<std::string, CacheLine>> fullCache;
};
#endif // !_CACHE_H_

