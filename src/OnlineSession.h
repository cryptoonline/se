//
//  OnlineSession.h
//  BlindStorage
//

#ifndef __BlindStorage__OnlineSession__
#define __BlindStorage__OnlineSession__

#include <stdint.h>
#include <iostream>

#include <csignal>

#include <algorithm>
using std::max;

#include <unordered_map>
using std::unordered_map;

#include "Communicator.h"
#include "PRSubset.h"
#include "Ddisk.h"
#include "PRF.h"
#include "Debug.h"
#include "DiskCommunicator.h"
//#include "LZO.h"
#include "FileHandling.h"

class OnlineSession {
private:
	DiskCommunicator dcomm;
	string filename;
	fileID fid;
	PRSubset prSubset;
	size_t filesize;

	b_index_t numBlocks;
	b_index_t numFileBlocks;
	
	vector<DataBlock> blocks;
	vector<DataBlock> fileBlocks;
	vector<uint32_t> updatedFileBlocksIndices;
	
	void readT(t_index_t TRecordIndex, byte block[]);
	void writeT(t_index_t TRecordIndex, byte block[]);

//	bool readOT(string filename, PRSubset &prSubset);
//	void writeOT(string filename, PRSubset prSubset);

	void readD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]);
	void writeD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]);

	static double diskAccessTime;

	void firstDAccess(string filename, double& diskTime);
	void secondDAccess(string filename, double& diskTime);

	void updateAccess(string filename, double& diskTime);
/*
	unordered_map<uint64_t, PRSubset> localT;
	void loadLocalT();
	void writeLocalTBack();
	HashMAC localTHash;
*/
public:
	OnlineSession();
	~OnlineSession();
	size_t updateRead(string filename, byte*& file, size_t bytesToAdd, double& diskTime);
	void updateWrite(string filename, byte updatedFile[], size_t sizeAfterUpdate, double& diskTime);
	/*! Use homorphic encryption to support this */
	void rename();
	
	double getDiskAccessTime();
	void resetDiskAccessTime();
	void updateWithInsecureAppend(string filename, byte*& file, double& diskTime);

//	size_t updateReadWithLocalT(string filename, byte*&, size_t updatedFileSize);
//	void updateWriteWithLocalT(string filename, byte updatedFile[], size_t updatedFileSize);
};

#endif /* defined(__BlindStorage__Dfile__) */
