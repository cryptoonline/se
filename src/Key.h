//
// Key.h
// BlindStorage
//

#ifndef __BlindStorage__Key__
#define __BlindStorage__Key__

#include "parameters.h"

#include <iostream>
using std::cerr;

#include <fstream>

#include "cryptopp562/osrng.h"
using CryptoPP::AutoSeededRandomPool;

using std::ifstream;
using std::ofstream;
using std::string;

class Key{
private:
	int keySize;
	char* key;

	std::ifstream keyfileRead;
	std::ofstream keyfileWrite;
	string filename;

	void generate();
	bool isKeyPresentOnDisk();
	void save();
	void load();
	void setup();
	
public:
	Key(string filename, int keySize);
	~Key();
	void get(byte key[]);
};

#endif /* defined(__BlindStorage__Key__) */
