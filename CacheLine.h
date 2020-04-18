#ifndef _CACHE_LINE_H_
#define _CACHE_LINE_H_

#include <string>
#include <vector>

class CacheLine {
public:
	CacheLine();
	CacheLine(int blockSize);

private:
	int validBit;
	std::vector<std::string> data;
};

#endif // !_LINE_H_
