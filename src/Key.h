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

#include <vector>
using std::vector;

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

using std::ifstream;
using std::ofstream;
using std::string;

#include "Debug.h"

class Key{
private:
	int keySize;
//	char* key;
//	byte key[keySize];
	vector<byte> key;

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
