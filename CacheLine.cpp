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

std::string CacheLine::ReadFromCacheLine(int blockOffset)
{
	return data.at(blockOffset);
}

void CacheLine::SetData(std::vector<std::string> dataToSet)
{
	data = dataToSet;
}
