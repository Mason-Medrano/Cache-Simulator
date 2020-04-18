#include "CacheLine.h"
using namespace std;

CacheLine::CacheLine()
	: validBit(0), data(0)
{

}

CacheLine::CacheLine(int blockSize)
	: validBit(1), data(blockSize)
{
	
}

CacheLine::CacheLine(std::vector<std::string> inputLine)
{

}

std::string CacheLine::ReadFromCacheLine(int blockOffset)
{
	return data.at(blockOffset);
}
