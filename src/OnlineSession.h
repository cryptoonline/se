//
//  OnlineSession.h
//  BlindStorage
//

#ifndef __BlindStorage__OnlineSession__
#define __BlindStorage__OnlineSession__

#include <stdint.h>
#include <iostream>
#include <algorithm>
using std::max;

#include "Communicator.h"
#include "PRSubset.h"
#include "Ddisk.h"
#include "PRF.h"
#include "TBlock.h"
#include "Tdisk.h"
#include "Debug.h"
#include "Cri.h"
#include "DiskCommunicator.h"

class OnlineSession {
private:
	DiskCommunicator dcomm;
	string filename;
	fileID fid;
	fileID criFid;
	TBlock tBlock;
	PRSubset criPRSubset;
	PRSubset filePRSubset;
	CRIBlock criBlock;
	CRI cri;
	b_index_t numBlocks;
	b_index_t numFileBlocks;
	vector<DataBlock> blocks;
	vector<DataBlock> fileBlocks;
	vector<uint32_t> updatedFileBlocksIndices;
	vector<DataBlock> criBlocks;
	b_index_t previousCriSize;	
	vector<DataBlock> fileCriBlocks;
	vector<uint32_t> updatedCriFileBlocksIndices;
	uint32_t criBlockIndex;
	
	void readT(t_index_t TRecordIndex, byte block[]);
	void writeT(t_index_t TRecordIndex, byte block[]);

	void readD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]);
	void writeD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]);

	void readCRI(PRSubset& prSubset, CRI& cri);
	void writeCRI();

	int retrieveTBlock();
	int retrieveCRIBlock();
	size_t retrieveDBlocks(b_index_t numBlocksToWrite = 0);
	void extractFileBlocks();
	

public:
	OnlineSession();
	~OnlineSession();
	size_t read(string filename, byte*& file, b_index_t numBlocksToRead = 0);
	size_t updateRead(string filename, byte*& file, size_t newContentSize);
	void updateWrite(string filename, byte updatedFile[], size_t updatedFileSize);
	void write(string filename, byte contents[], size_t size);
	void remove(string filename);
	/*! Use homorphic encryption to support this */
	void rename();
};

#endif /* defined(__BlindStorage__Dfile__) */
