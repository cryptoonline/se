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
#include "fileID.h"

class Cri{
private:
	DataBlock** blocks;
	DataBlock** extractedBlocks;
	PRSubset* prSubset;
	uint32_t* blocksLocations;
	unsigned char** ciphertextBlocks;
	unsigned char** plaintextBlocks;
	unsigned char** extractedPlaintextBlocks;
	unsigned char** entries;
	
	fileID* fid;

	uint32_t numBlocks;
	uint32_t numEntries;

	void decrypt();
	
	void makeCriBlock(unsigned char* criEntry);
	int32_t getEmptyCriEntry();

public:
	Cri();

	void addTBlock(TBlock& tblock);
	void addFile(unsigned char** file, fileID& fid);
	uint32_t* getBlocksLocations();
	uint32_t getNumBlocks();
	unsigned char* getEntry(int32_t index);	
	int32_t searchFID(unsigned char* arrayToSearchFor);
	void writeCriEntry(unsigned char* D, unsigned char* criEntry);
	PRSubset* getCRIPrSubset();
};

#endif /* defined(__BlindStorage__Cri__) */
