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

#include <cstdio>

class FileStore {
private:

public:
	FileStore();
	~FileStore();
	void get(string srcPath, string dstPath);
	void put(string srcPath, string dstPath);
	void copy(string srcPath, string dstPath);
	void remove(string path);
};

#endif /* defined(__SearchableEncryption__FileStore__) */
