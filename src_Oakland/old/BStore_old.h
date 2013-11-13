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

typedef uint64_t docid_t;

class BStore{
public:

	BStore(Communicator &communicator);
	BStore(Communicator &communicator, string path);
	BStore(unordered_map< string, unordered_set<uint64_t> > & map);
	~BStore();
	void upload();

	bool  read(string filename, vector<unsigned char>& fileContents);
	void write(string filename, unsigned char* filedata, uint32_t size);
	unsigned char* update(string filename, unsigned char* updateFiledata, uint32_t size);
	void del(string filename);
	void rename(string filename);
	ifstream::pos_type readFileSize(string filename);

private:
	Communicator communicator;
	vector<string> filesList;
	void readFilesFromDirectory(string path);
	Ddisk D;
	Tdisk T;
};

#endif /* defined(__BlindStorge__BStore__) */

