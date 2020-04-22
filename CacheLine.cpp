#include <iostream>
#include "CacheLine.h"
using namespace std;

CacheLine::CacheLine()
	: validBit(0), dirtyBit(0), data(0)
{

}

CacheLine::CacheLine(int blockSize)
	: validBit(0), dirtyBit(0), data(blockSize, "00")
{
	
}

CacheLine::CacheLine(std::vector<std::string> dataToSet)
	: validBit(1), dirtyBit(0)
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

void CacheLine::PrintCacheLine()
{
	for (int i = 0; i < data.size(); ++i) {
		cout << data.at(i) << " ";
	}
}

void CacheLine::PrintCacheLineToFile(ofstream& ofs)
{
	for (int i = 0; i < data.size(); ++i) {
		ofs << data.at(i) << " ";
	}
}

void CacheLine::AssertValidBit()
{
	this->validBit = 1;
}

void CacheLine::FlipDirtyBit()
{
	if (dirtyBit == 0) {
		dirtyBit = 1;
	}
	else if (dirtyBit == 1) {
		dirtyBit = 0;
	}
}

void CacheLine::SetTimer()
{
	timeInCache = chrono::steady_clock::now();
}

int CacheLine::GetValidBit()
{
	return validBit;
}

int CacheLine::GetDirtyBit()
{
	return dirtyBit;
}

std::chrono::steady_clock::time_point CacheLine::GetTime()
{
	return timeInCache;
}
