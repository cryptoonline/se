//
//  OnlineSession.h
//  BlindStorage
//

#ifndef __BlindStorage__Dfile__
#define __BlindStorage__Dfile__

#include <stdint.h>
#include <iostream>
#include "Communicator.h"
#include "PRSubset.h"
#include "Ddisk.h"
#include "PRF.h"
#include "TBlock.h"
#include "Tdisk.h"
#include "Debug.h"
#include "Cri.h"
#include "DataStructures.h"

using namespace std;
using std::reverse_iterator;

class OnlineSession {
private:
//	Communicator communicator;
	fileID fid;
	TBlock tBlock;
	PRSubset criPRSubset;
	PRSubset filePRSubset;
	CRIBlock criBlock;
	CRI cri;
	b_index_t numBlocks;
	b_index_t numFileBlocks;
	vector<DataBlock> blocks;
	vector<DataBlock> fileBlocks;
	  
	void readT(t_index_t TRecordIndex, byte block[]);
	void writeT(t_index_t TRecordIndex, byte block[]);

	void readD(b_index_t blockIndices[], b_index_t numBlocks, byte[][BLOCK_SIZE]);
	void writeD(b_index_t blockIndices[], b_index_t numBlocks, byte[][BLOCK_SIZE]);

	void parseCRI(PRSubset& prSubset, CRIBlock& block);

public:
	OnlineSession();
	~OnlineSession();
	bool read(string filename, byte* contents);
	void write(string filename, byte contents[], size_t size);
	void remove(string filename);
	/*! Use homorphic encryption to support this */
	void rename();
};

#endif /* defined(__BlindStorage__Dfile__) */
