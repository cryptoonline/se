//
// CRI.h
// BlindStorage
//

#ifndef __BlindStorage__CRI__
#define __BlindStorage__CRI__

#include <stdio.h>
#include <vector>
using std::vector;

#include "DataBlock.h"
#include "TBlock.h"
#include "CRIBlock.h"
#include "PRSubset.h"
#include "parameters.h"
#include "Debug.h"
#include "fileID.h"

class CRI{
private:
	higherfid_t higherfid;
	vector<CRIBlock> blocks;
	bool empty;
		
public:
	CRI();
	CRI(higherfid_t higherfid);
	~CRI();

	bool isEmpty();
//	void parseBytes(vector<byte>& blocksBytes);
	void parseBytes(byte blocksBytes[], uint32_t size);
	void makeBytes(vector<byte>& blocksBytes, string function);
	void makeBytes(byte* blockBytes);
	void addFile(prSubsetSize_t size, prSubsetSeed_t seed, size_t filesize, byte lowerFid[]);
	void updateFile(prSubsetSize_t size, prSubsetSeed_t seed, size_t filesize, uint32_t blockIndex);
	uint32_t search(fileID fid, CRIBlock& block);
	uint32_t search(byte lowerFid[], CRIBlock& block);
	criSize_t size();
};

#endif /* defined(__BlindStorage__CRI__) */
