//
// BStore.h
// BlindStorage
//

#ifndef __BlindStorage__BStore__
#define __BlindStorage__BStore__

#include <iostream>
#include <dirent.h>
#include <vector>
#include <sys/stat.h>
#include "Ddisk.h"
#include "Communicator.h"
#include "OnlineSession.h"
#include "Tdisk.h"
#include "LZO.h"
#include "boost/filesystem.hpp"

//#include "tr1/unordered_map"
//using std::tr1::unordered_map;

//#include "tr1/unordered_set"
//using std::tr1::unordered_set;

using namespace std;

class BStore{
public:
	BStore();
	BStore(string directoryPath);
	void add(string filename, byte fileBytes[], size_t size);
	void finalize(double& execTime);
	~BStore();

private:
	b_index_t totalFileBlocks;
//	byte* allFileBytes;
	void readFileNamesFromDirectory(string path, vector<string>& filesList);
	void readFile(string path, byte fileBytes[], size_t size);
	size_t readFileSize(string path);
	Ddisk D;
	Tdisk T;

	size_t compressionAdvntage;
};

#endif /* defined(__BlindStorge__BStore__) */

