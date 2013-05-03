//
// Key.h
// BlindStorage
//

#ifndef __BlindStorage__Key__
#define __BlindStorage__Key__

#include "parameters.h"
#include "AES.h"
#include <iostream>
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::string;

class Key{
private:
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
	Key(string filename);
	~Key();
	char* get();
};

#endif /* defined(__BlindStorage__Key__) */
