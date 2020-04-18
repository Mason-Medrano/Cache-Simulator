#ifndef _CACHE_LINE_H_
#define _CACHE_LINE_H_

#include <string>
#include <vector>

class CacheLine {
public:
	CacheLine();
	CacheLine(int blockSize);
	std::string ReadFromCacheLine(int blockOffset);
	void SetData(std::vector<std::string> dataToSet);
private:
	int validBit;
	std::vector<std::string> data;
};

#endif // !_LINE_H_
