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
#include "LZO.h"
#include "boost/filesystem.hpp"

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
	void readFileNamesFromDirectory(string path, vector<string>& filesList);
	void readFile(string path, byte fileBytes[], size_t size);
	size_t readFileSize(string path);
	Ddisk D;
};

#endif /* defined(__BlindStorge__BStore__) */

