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
#include "boost/filesystem.hpp"

#include "tr1/unordered_map"
using std::tr1::unordered_map;

#include "tr1/unordered_set"
using std::tr1::unordered_set;

using namespace std;

class BStore{
public:
	BStore();
	BStore(string directoryPath);
	~BStore();

private:
	void readFileNamesFromDirectory(string path, vector<string>& filesList);
	void readFile(string path, byte contents[], b_index_t numBlocks);
	size_t readFileSize(string path);
	Ddisk D;
	Tdisk T;
};

#endif /* defined(__BlindStorge__BStore__) */

