//
// Cri.h
// BlindStorage
//

#ifndef __BlindStorage__Cri__
#define __BlindStorage__Cri__

#include "DataBlock.h"
#include "TBlock.h"
#include "PRSubset.h"
#include "parameters.h"
#include "Debug.h"

class Cri{
private:
	DataBlock** blocks;
	PRSubset* prSubset;
	uint32_t* blocksLocations;
	unsigned char** ciphertextBlocks;
	unsigned char** plaintextBlocks;
	unsigned char** entries;

	uint32_t numBlocks;
	uint32_t numEntries;

	void decrypt();
	
public:
	Cri();

	void addTBlock(TBlock& tblock);
	void addFile(unsigned char** file);
	uint32_t* getBlocksLocations();
	uint32_t getNumBlocks();
	unsigned char* getEntry(int32_t index);	
	int32_t searchFID(unsigned char* arrayToSearchFor);
};

#endif /* defined(__BlindStorage__Cri__) */
