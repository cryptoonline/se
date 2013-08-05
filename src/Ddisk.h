//
//  Ddisk.h
//  BlindStorage
//

#ifndef __BlindStorage__Ddisk__
#define __BlindStorage__Ddisk__

#include <stdint.h>
#include <iostream>
#include <fstream>

#include "Communicator.h"
#include "PRSubset.h"
#include "fileID.h"
#include "DataBlock.h"
#include "Key.h"
#include "Debug.h"

class Ddisk {
private:
	b_index_t numBlocks;
	b_index_t usedBlocks;
//	vector<DataBlock> D;
//	DataBlock D[TOTAL_BLOCKS];
	DataBlock** D;
//	byte* zeroBytes;
	void makeBlocks(byte bytes[], size_t size, fileID fid, vector<b_index_t>& emptyBlocks, bool isCRI);
	void getEmptyBlocks(PRSubset prSubset, vector<b_index_t>& emptyBlocks);
	b_index_t numOccupiedBlocks;

public:
	Ddisk(b_index_t numBlocks = TOTAL_BLOCKS);
	~Ddisk();
	void addFile(byte bytes[], size_t size, fileID fid, PRSubset prSubset, bool isCRI = false);
	void encryptEmptyBlocks();
	void getBlock(b_index_t index, DataBlock& block);
	void writeToDisk();
	b_index_t getNumOccupiedBlocks();
};

#endif /* defined(__BlindStorage__Ddisk__) */
