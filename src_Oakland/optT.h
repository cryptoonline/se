//
// optT.h 
// BlindStorage
//

#ifndef __BlindStorage__optT__
#define __BlindStorage__optT__

#include <unordered_map>
#include <iostream>
#include <fstream>

#include "parameters.h"
#include "HashMAC.h"
#include "PRSubset.h"
#include "Key.h"
#include "Debug.h"

using std::unordered_map;
using std::ofstream;
using std::ifstream;

using std::cout;
using std::endl;

class optT{
private:
	unordered_map<uint64_t, PRSubset> T;
	HashMAC hash;

	byte key[HMAC_KEY_SIZE];

public:
	optT();
	~optT();
	void addFile(string filename, size_t filesize, PRSubset prSubset);
	void writeToDisk();
};

#endif /* defined(__BlindStorage__optT__) */
