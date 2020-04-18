#include <math.h>
#include <iostream>
#include <sstream>
#include <stdlib.h> 
#include "Cache.h"
using namespace std;

const int addressLength = 8;

Cache::Cache() 
	: C(-1),  E(-1), B(-1), replacePolicy(-1), writeHitPolicy(-1), writeMissPolicy(-1)
{

}

Cache::Cache(int cacheSize, int blockSize, int linesPerSet, int replacePolicyInput, int writeHPInput, int writeMPInput, MainMemory* ptrToRAM)
	: C(cacheSize), E(linesPerSet), B(blockSize), replacePolicy(replacePolicyInput), writeHitPolicy(writeHPInput), 
	  writeMissPolicy(writeMPInput), RAM(ptrToRAM)
	 
{
	S = C / (E * B);
	s = log2(S);
	b = log2(B);
	t = addressLength - (s + b);

	fullCache.resize(S);
}

std::string Cache::CacheRead(string binaryAddress, string hexAddressToPrint)
{
	ostringstream temp;
	string hexTag = "";
	vector<string> readLine;
	
	string tagBits = binaryAddress.substr(0, t);
	string setIndex = binaryAddress.substr((0 + t), s);
	string blockOffSet = binaryAddress.substr((0 + t + s), b);

	int setIndexNumeric = BinaryToDecimal(setIndex);
	int blockOffsetNumeric = BinaryToDecimal(blockOffSet);
	int tagBitsNumeric = BinaryToDecimal(tagBits);
	int memoryAddress = BinaryToDecimal(binaryAddress);

	temp << hex << tagBitsNumeric;
	hexTag = temp.str();

	cout << "set:" << setIndexNumeric << endl;
	cout << "tag:" << hexTag << endl;


	map<string, CacheLine>::iterator it;
	it = fullCache.at(setIndexNumeric).find(tagBits);

	if (it == fullCache.at(setIndexNumeric).end()) {
		cout << "hit:no" << endl;
		
		if (fullCache.at(setIndexNumeric).size() != E) {
			readLine = RAM->ReadLine(memoryAddress, B);
			
			pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));

			fullCache.at(setIndexNumeric).insert(currentLine);

			return fullCache.at(setIndexNumeric).at(tagBits).ReadFromCacheLine(blockOffsetNumeric);
		}
		else if (replacePolicy == 1 && fullCache.at(setIndexNumeric).size() == E) {
			int maxIndex = (fullCache.at(setIndexNumeric).size() - 1);
			int replacementIndex = rand() % maxIndex;
			it = fullCache.at(setIndexNumeric).begin();
			advance(it, replacementIndex);

			readLine = RAM->ReadLine(memoryAddress, B);

			pair<string, CacheLine> currentLine;
			currentLine = *it;

			currentLine.first = tagBits;
			currentLine.second.SetData(readLine);

			return currentLine.second.ReadFromCacheLine(blockOffsetNumeric);
		}
	}
	else {
		cout << "hit:yes" << endl;
		cout << "eviction_line:-1" << endl;
		it->second.ReadFromCacheLine(blockOffsetNumeric);
	}
}

int Cache::BinaryToDecimal(std::string binaryNumber)
{
	int decimalNum = 0;
	int multiplier = 0;
	
	for (int i = (binaryNumber.length() - 1); i >= 0 ; --i) {
		if (binaryNumber.at(i) == '0') {
			++multiplier;
		}
		else if (binaryNumber.at(i) == '1') {
			decimalNum += (binaryNumber.at(i) * pow(10, multiplier));
			++multiplier;
		}
	}

	return decimalNum;
}
