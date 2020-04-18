#include <math.h>
#include "Cache.h"
using namespace std;

const int addressLength = 8;

Cache::Cache() 
	: C(-1),  E(-1), B(-1), replacePolicy(-1), writeHitPolicy(-1), writeMissPolicy(-1)
{

}

Cache::Cache(int cacheSize, int blockSize, int linesPerSet, int replacePolicyInput, int writeHPInput, int writeMPInput)
	: C(cacheSize), E(linesPerSet), B(blockSize), replacePolicy(replacePolicyInput), writeHitPolicy(writeHPInput), 
	 writeMissPolicy(writeMPInput)
{
	S = C / (E * B);
	s = log2(S);
	b = log2(B);
	t = addressLength - (s + b);

	fullCache.resize(S);
}
