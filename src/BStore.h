//
// BStore.h
// BlindStorage
//

#ifndef __BlindStorage__BStore.h__
#define __BlindStorage__BStore.h__

#include <iostream>
#include <dirent.h>
#include <vector>
#include <sys/stat.h>
#include "Ddisk.h"
#include "Communicator.h"
#include "OnlineSession.h"
#include "TDisk.h"

using namespace std;

class BStore{
public:
	BStore(Communicator &communicator, string path);
	~BStore();
	void upload();
	unsigned char* read(string filename);
	void write(string filename, unsigned char* filedata);
	unsigned char* update(string filename, unsigned char* updateFiledata);
	void del(string filename);
	void rename(string filename);
	size_t readFileSize(string filename);

private:
	Communicator communicator;
	vector<string> filesList;
	void readFilesFromDirectory(string directoryPath);
	Ddisk D;
	Tdisk T;
};

#endif /* defined(__BlindStorge__BStore__) */

