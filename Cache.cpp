#include <math.h>
#include <iostream>
#include <iomanip> 
#include <sstream>
#include <fstream>
#include <stdlib.h> 
#include <chrono>
#include "Cache.h"
using namespace std;

const int addressLength = 8;

Cache::Cache() 
	: C(-1),  E(-1), B(-1), replacePolicy(-1), writeHitPolicy(-1), 
	writeMissPolicy(-1), RAM(NULL), cacheHits(0), cacheMisses(0)
{

}

Cache::Cache(int cacheSize, int blockSize, int linesPerSet, int replacePolicyInput, int writeHPInput, int writeMPInput, MainMemory* ptrToRAM)
	: C(cacheSize), E(linesPerSet), B(blockSize), replacePolicy(replacePolicyInput), writeHitPolicy(writeHPInput), 
	  writeMissPolicy(writeMPInput), RAM(ptrToRAM), cacheHits(0), cacheMisses(0)
	 
{
	S = C / (E * B);
	s = log2(S);
	b = log2(B);
	t = addressLength - (s + b);

	fullCache.resize(S);
	for (int i = 0; i < fullCache.size(); ++i) {
		fullCache.at(i) = vector<pair<string, CacheLine>>(E, pair<string, CacheLine>("", (B)));
	}
}

std::string Cache::CacheRead(string binaryAddress, string hexAddressToPrint)
{
	ostringstream temp;
	string hexTag = "";
	vector<string> readLine;
	bool foundLine = false;
	bool setFull = false;
	int lineFoundIndex = 0;
	
	string tagBits = binaryAddress.substr(0, t);
	string setIndex = binaryAddress.substr((0 + t), s);
	string blockOffSet = binaryAddress.substr((0 + t + s), b);

	int setIndexNumeric = BinaryToDecimal(setIndex);
	int blockOffsetNumeric = BinaryToDecimal(blockOffSet);
	int tagBitsNumeric = BinaryToDecimal(tagBits);
	int memoryAddress = BinaryToDecimal(binaryAddress);
	int adjustedMemAddress = ConvertToRAMAddress(binaryAddress);

	temp << hex << tagBitsNumeric;
	hexTag = temp.str();
	for (int i = 0; i < hexTag.length(); ++i) {
		hexTag.at(i) = toupper(hexTag.at(i));
	}

	cout << "set:" << setIndexNumeric << endl;
	cout << "tag:" << setw(2) << setfill('0') << hexTag << endl;

	std::vector<std::pair<string, CacheLine>>* set;
	set = &(fullCache.at(setIndexNumeric));

	for (int i = 0; i < set->size(); ++i) {
		if (set->at(i).first == tagBits && set->at(i).second.GetValidBit() == 1) {
			foundLine = true;
			lineFoundIndex = i;
			break;
		}
	}

	if (!foundLine) {
		setFull = (set->at(set->size() - 1).second.GetValidBit() == 1);
		cout << "hit:no" << endl;
		++cacheMisses;
		
		if (!setFull) {
			readLine = RAM->ReadLine(adjustedMemAddress, B);
			
			int indexToReturn = 0;
			pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
			currentLine.second.SetSourceAddress(adjustedMemAddress);

			currentLine.second.SetTimer();

			for (int i = 0; i < set->size(); ++i) {
				if (set->at(i).second.GetValidBit() == 0) {
					indexToReturn = i;
					set->at(i) = currentLine;
					break;
				}
			}

			return set->at(indexToReturn).second.ReadFromCacheLine(blockOffsetNumeric);
		}
		else if (replacePolicy == 1 && setFull) {
			srand(time(0));
			int maxIndex = (set->size());
			int replacementIndex = rand() % maxIndex;
			cout << "eviction_line:" << replacementIndex << endl;

			readLine = RAM->ReadLine(adjustedMemAddress, B);

			pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
			currentLine.second.SetSourceAddress(adjustedMemAddress);

			currentLine.second.SetTimer();

			set->at(replacementIndex) = currentLine;

			return set->at(replacementIndex).second.ReadFromCacheLine(blockOffsetNumeric);
		}
		else if (replacePolicy == 2 && setFull) {
			int leastIndex = -1;
			std::chrono::steady_clock::time_point leastTime = set->at(0).second.GetTime();
			leastIndex = 0;

			for (int i = 1; i < set->size(); ++i) {
				if (set->at(i).second.GetTime() < leastTime) {
					leastTime = set->at(i).second.GetTime();
					leastIndex = i;
				}
			}

			cout << "eviction_line:" << leastIndex << endl;

			readLine = RAM->ReadLine(adjustedMemAddress, B);

			pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
			currentLine.second.SetSourceAddress(adjustedMemAddress);

			currentLine.second.SetTimer();

			set->at(leastIndex) = currentLine;

			return set->at(leastIndex).second.ReadFromCacheLine(blockOffsetNumeric);
		}
	}
	else {
		cout << "hit:yes" << endl;
		cout << "eviction_line:-1" << endl;
		++cacheHits;
		set->at(lineFoundIndex).second.SetTimer();
		return set->at(lineFoundIndex).second.ReadFromCacheLine(blockOffsetNumeric);
	}
}

void Cache::CacheWrite(string binaryAddress, string hexToStore, string hexAddressToPrint)
{
	ostringstream temp;
	string hexTag = "";
	vector<string> readLine;
	bool foundLine = false;
	bool setFull = false;
	int lineFoundIndex = 0;
	int dirtyBitToPrint = 0;

	string tagBits = binaryAddress.substr(0, t);
	string setIndex = binaryAddress.substr((0 + t), s);
	string blockOffSet = binaryAddress.substr((0 + t + s), b);

	int setIndexNumeric = BinaryToDecimal(setIndex);
	int blockOffsetNumeric = BinaryToDecimal(blockOffSet);
	int tagBitsNumeric = BinaryToDecimal(tagBits);
	int memoryAddress = BinaryToDecimal(binaryAddress);
	int adjustedMemAddress = ConvertToRAMAddress(binaryAddress);

	temp << hex << tagBitsNumeric;
	hexTag = temp.str();
	for (int i = 0; i < hexTag.length(); ++i) {
		hexTag.at(i) = toupper(hexTag.at(i));
	}

	cout << "set:" << setIndexNumeric << endl;
	cout << "tag:" << setw(2) << setfill ('0') << hexTag << endl;

	std::vector<std::pair<string, CacheLine>>* set;
	set = &(fullCache.at(setIndexNumeric));

	for (int i = 0; i < set->size(); ++i) {
		if (set->at(i).first == tagBits && set->at(i).second.GetValidBit() == 1) {
			foundLine = true;
			lineFoundIndex = i;
			break;
		}
	}

	if (!foundLine) {
		setFull = (set->at(set->size() - 1).second.GetValidBit() == 1);
		cout << "write_hit:no" << endl;
		++cacheMisses;

		if (!setFull) {
			if (writeMissPolicy == 1) {
				readLine = RAM->ReadLine(adjustedMemAddress, B);
				int indexToWrite = 0;
				pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
				currentLine.second.SetSourceAddress(adjustedMemAddress);

				for (int i = 0; i < set->size(); ++i) {
					if (set->at(i).second.GetValidBit() == 0) {
						indexToWrite = i;
						set->at(i) = currentLine;
						break;
					}
				}

				if (writeHitPolicy == 1) {
					set->at(indexToWrite).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					set->at(indexToWrite).second.SetDirtyBit(0);
					dirtyBitToPrint = 0;
					set->at(indexToWrite).second.SetTimer();

					RAM->Write(memoryAddress, hexToStore);
				}
				else if (writeHitPolicy == 2) {
					set->at(indexToWrite).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					set->at(indexToWrite).second.SetDirtyBit(1);
					dirtyBitToPrint = 1;
					set->at(indexToWrite).second.SetTimer();
				}
			}
			else if (writeMissPolicy == 2) {
				RAM->Write(memoryAddress, hexToStore);
				dirtyBitToPrint = 0;
			}
		}
		else if (replacePolicy == 1 && setFull) {
			srand(time(0));
			int maxIndex = (set->size());
			int replacementIndex = rand() % maxIndex;
			cout << "eviction_line:" << replacementIndex << endl;

			if (writeMissPolicy == 1) {
				readLine = RAM->ReadLine(adjustedMemAddress, B);
				pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
				currentLine.second.SetSourceAddress(adjustedMemAddress);
				if (writeHitPolicy == 2) {
					RAM->WriteLine(set->at(replacementIndex).second.GetSourceAddress(), B, set->at(replacementIndex).second.GetCacheLine());
				}

				set->at(replacementIndex) = currentLine;

				if (writeHitPolicy == 1) {
					set->at(replacementIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					set->at(replacementIndex).second.SetDirtyBit(0);
					dirtyBitToPrint = 0;
					set->at(replacementIndex).second.SetTimer();

					RAM->Write(memoryAddress, hexToStore);
				}
				else if (writeHitPolicy == 2) {
					set->at(replacementIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					set->at(replacementIndex).second.SetDirtyBit(1);
					dirtyBitToPrint = 1;
					set->at(replacementIndex).second.SetTimer();
				}
			}
			else if (writeMissPolicy == 2) {
				RAM->Write(memoryAddress, hexToStore);
				dirtyBitToPrint = 0;
			}
		}
		else if (replacePolicy == 2 && setFull) {
			int leastIndex = -1;
			std::chrono::steady_clock::time_point leastTime = set->at(0).second.GetTime();
			leastIndex = 0;

			for (int i = 1; i < set->size(); ++i) {
				if (set->at(i).second.GetTime() < leastTime) {
					leastTime = set->at(i).second.GetTime();
					leastIndex = i;
				}
			}

			cout << "eviction_line:" << leastIndex << endl;

			if (writeMissPolicy == 1) {
				readLine = RAM->ReadLine(adjustedMemAddress, B);
				pair<string, CacheLine> currentLine(tagBits, CacheLine(readLine));
				currentLine.second.SetSourceAddress(adjustedMemAddress);
				if (writeHitPolicy == 2) {
					RAM->WriteLine(set->at(leastIndex).second.GetSourceAddress(), B, set->at(leastIndex).second.GetCacheLine());
				}
				set->at(leastIndex) = currentLine;

				if (writeHitPolicy == 1) {
					set->at(leastIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					set->at(leastIndex).second.SetDirtyBit(0);
					dirtyBitToPrint = 0;
					set->at(leastIndex).second.SetTimer();

					RAM->Write(memoryAddress, hexToStore);
				}
				else if (writeHitPolicy == 2) {
					set->at(leastIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
					set->at(leastIndex).second.SetDirtyBit(1);
					dirtyBitToPrint = 1;
					set->at(leastIndex).second.SetTimer();
				}
			}
			else if (writeMissPolicy == 2) {
				RAM->Write(memoryAddress, hexToStore);
				dirtyBitToPrint = 0;
			}
		}

		cout << "ram_address:" << hexAddressToPrint << endl;
		cout << "data:0x" << hexToStore << endl;
		cout << "dirty_bit:" << dirtyBitToPrint << endl;
	}
	else {
		cout << "write_hit:yes" << endl;
		cout << "eviction_line:-1" << endl;
		cout << "ram_address:-1" << endl;
		cout << "data:0x" << hexToStore << endl;
		++cacheHits;

		if (writeHitPolicy == 1) {
			set->at(lineFoundIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
			set->at(lineFoundIndex).second.SetDirtyBit(0);
			dirtyBitToPrint = 0;
			set->at(lineFoundIndex).second.SetTimer();

			RAM->Write(memoryAddress, hexToStore);
		}
		else if (writeHitPolicy == 2) {
			set->at(lineFoundIndex).second.WriteToCacheLine(blockOffsetNumeric, hexToStore);
			set->at(lineFoundIndex).second.SetDirtyBit(1);
			dirtyBitToPrint = 1;
			set->at(lineFoundIndex).second.SetTimer();
		}

		cout << "dirty_bit:" << dirtyBitToPrint << endl;
	}
}

void Cache::CacheFlush()
{
	for (int i = 0; i < fullCache.size(); ++i) {
		for (int j = 0; j < fullCache.at(i).size(); ++j) {
			if (fullCache.at(i).at(j).second.GetDirtyBit() == 1) {
				RAM->WriteLine(fullCache.at(i).at(j).second.GetSourceAddress(), B, fullCache.at(i).at(j).second.GetCacheLine());
			}
		}
		fullCache.at(i) = vector<pair<string, CacheLine>>(E, pair<string, CacheLine>("", (B)));
	}
}

void Cache::CacheDump()
{
	ofstream outFS;
	outFS.open("cache.txt");
	
	std::vector<std::pair<std::string, CacheLine>>* set;
	for (int i = 0; i < fullCache.size(); ++i) {
		set = &(fullCache.at(i));
		for (int j = 0; j < set->size(); ++j) {
			set->at(j).second.PrintCacheLineToFile(outFS);
			outFS << endl;
		}
	}

	outFS.close();
}

void Cache::PrintCacheContents()
{
	cout << "cache_size:" << C << endl;
	cout << "data_block_size:" << B << endl;
	cout << "associativity:" << E << endl;

	cout << "replacement_policy:";
	if (replacePolicy == 1) {
		cout << "random_replacement" << endl;
	}
	else if (replacePolicy == 2) {
		cout << "least_recently_used" << endl;
	}

	cout << "write_hit_policy:";
	if (writeHitPolicy == 1) {
		cout << "write_through" << endl;
	}
	else if (writeHitPolicy == 2) {
		cout << "write_back" << endl;
	}

	cout << "write_miss_policy:";
	if (writeMissPolicy == 1) {
		cout << "write_allocate" << endl;
	}
	else if (writeMissPolicy == 2) {
		cout << "no_write_allocate" << endl;
	}

	cout << "number_of_cache_hits:" << cacheHits << endl;
	cout << "number_of_cache_misses:" << cacheMisses << endl;

	cout << "cache_content:" << endl;
	std::vector<std::pair<std::string, CacheLine>>* set;
	for (int i = 0; i < fullCache.size(); ++i) {
		set = &(fullCache.at(i));
		for (int j = 0; j < set->size(); ++j) {
			cout << set->at(j).second.GetValidBit() << " ";
			cout << set->at(j).second.GetDirtyBit() << " ";
			cout << setw(2) << setfill('0') << BinaryToHex(set->at(j).first) << " ";
			set->at(j).second.PrintCacheLine();
			cout << endl;
		}
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
			decimalNum += (1 * pow(2, multiplier));
			++multiplier;
		}
	}

	return decimalNum;
}

std::string Cache::BinaryToHex(std::string binaryNumber)
{
	ostringstream outSS;
	string hex = "";
	int decimalNumber = 0;

	decimalNumber = BinaryToDecimal(binaryNumber);
	outSS << hex << decimalNumber;
	hex = outSS.str();
	MakeStringUppercase(hex);

	return hex;
}

void Cache::MakeStringUppercase(std::string& stringToChange)
{
	for (int i = 0; i < stringToChange.length(); ++i) {
		stringToChange.at(i) = toupper(stringToChange.at(i));
	}
}

int Cache::ConvertToRAMAddress(std::string addressString)
{
	int offset = 0;
	offset = b;

	for (int i = addressString.length() - 1; i >= 0 && offset > 0; --i) {
		addressString.at(i) = '0';
		--offset;
	}

	int ramAddress = 0;
	ramAddress = BinaryToDecimal(addressString);

	return ramAddress;
}
