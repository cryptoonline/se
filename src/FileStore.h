// 
// Searchable Encryption
// FileStore.h
//

#ifndef __SearchableEncryption__FileStore__
#define __SearchableEncryption__FileStore__

#include <iostream>
using std::string;
using std::cerr;
using std::endl;
using std::cout;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <unistd.h>

#include <cstdio>

#include <sys/stat.h>

#include <ctime>

#include "AES.h"
#include "helper.h"
#include "sse_parameters.h"

class FileStore {
private:
	void encrypt(byte contents[], size_t size);
	void decrypt(byte contents[], size_t size);

public:
	FileStore();
	~FileStore();
	size_t getNew(string filepath, byte*& contents);
	size_t get(string filename, byte*& contents, double& cryptoduration);
	void put(string filename, byte contents[], size_t size, double& cryptoduration);
	void copy(string srcPath, string dstPath);
	void remove(string filename);
	bool isFilePresent(string filename);
};

#endif /* defined(__SearchableEncryption__FileStore__) */
