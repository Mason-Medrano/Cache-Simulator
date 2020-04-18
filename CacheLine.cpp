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
