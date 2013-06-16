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
//	vector<DataBlock> D;
	DataBlock D[TOTAL_BLOCKS];
	void makeBlocks(byte bytes[], size_t size, fileID fid, vector<b_index_t>& emptyBlocks);
	void getEmptyBlocks(PRSubset prSubset, vector<b_index_t>& emptyBlocks);
    
public:
	Ddisk(b_index_t numBlocks = TOTAL_BLOCKS);
	void addBlocks(byte bytes[], size_t size, fileID fid, PRSubset prSubset);
	void encryptEmptyBlocks();
	void getBlock(b_index_t index, DataBlock& block);
};

#endif /* defined(__BlindStorage__Ddisk__) */
