#include "CacheLine.h"
using namespace std;

CacheLine::CacheLine()
	: validBit(0), data(0)
{

}

CacheLine::CacheLine(int blockSize)
	: validBit(0), data(blockSize)
{
	
}

CacheLine::CacheLine(std::vector<std::string> dataToSet)
{
	SetData(dataToSet);
}

std::string CacheLine::ReadFromCacheLine(int blockOffset)
{
	return data.at(blockOffset);
}

void CacheLine::SetData(std::vector<std::string> dataToSet)
{
	data = dataToSet;
}
