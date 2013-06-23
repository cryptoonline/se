//
// Cri.cpp
// BlindStorage
//

#include "Cri.h"

CRI::CRI(){
}

CRI::CRI(higherfid_t higherfid){
	this->higherfid = higherfid;
//	blocks.resize(2);
}

CRI::~CRI(){
}

void CRI::addFile(prSubsetSize_t size, prSubsetSeed_t seed, byte lowerFid[]){
	CRIBlock block;
	block.make(size, seed, lowerFid);
	blocks.push_back(block);
}

void CRI::makeBytes(byte* blocksBytes){
	int numPaddingBytes = MAX_BLOCK_DATA_SIZE - CRI_PER_BLOCK * CRI_BLOCK_SIZE;
	int counter = 0;
	int pointer = 0;
	for(vector<CRIBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it){
		byte block[CRI_BLOCK_SIZE];
		it->get(block);
		memcpy(&blocksBytes[pointer], block, CRI_BLOCK_SIZE);
		pointer += CRI_BLOCK_SIZE;
		if(counter++ % CRI_PER_BLOCK == 0)
			pointer += numPaddingBytes + TRAILER_SIZE;
	}
}

void CRI::makeBytes(vector<byte>& blocksBytes){
	int counter = 0;
	int numPaddingBytes = MAX_BLOCK_DATA_SIZE - CRI_PER_BLOCK * CRI_BLOCK_SIZE + TRAILER_SIZE;
	byte zeros[numPaddingBytes];
	memset(zeros, 0, numPaddingBytes);
	for(vector<CRIBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it){
		if(counter % CRI_PER_BLOCK == 0 && counter != 0)
			blocksBytes.insert(blocksBytes.end(), &zeros[0], &zeros[numPaddingBytes]);
		vector<byte> block;
		it->get(block);
		blocksBytes.insert(blocksBytes.end(), block.begin(), block.begin()+CRI_BLOCK_SIZE);
		counter++;
	}
}

// void CRI::parseBytes(vector<byte>& blocksBytes){
// 	for(int i = 0; i < blocksBytes.size(); i+=CRI_BLOCK_SIZE){
// 		byte block[CRI_BLOCK_SIZE];
// 		std::copy(blocksBytes.begin()+i*CRI_BLOCK_SIZE, blocksBytes.begin()+i*CRI_BLOCK_SIZE+CRI_BLOCK_SIZE, block);
// 		CRIBlock criBlock;
// 		criBlock.parse(block);
// 		blocks.push_back(criBlock);
// 	}
// }

void CRI::parseBytes(byte blocksBytes[], uint32_t size){
	int criCounter = 0;
	for(int i = 0; i < size; i+=CRI_BLOCK_SIZE){
		if(criCounter%CRI_PER_BLOCK == 0 && i!=0)
			i += MAX_BLOCK_DATA_SIZE - CRI_BLOCK_SIZE * CRI_PER_BLOCK + TRAILER_SIZE;
		criCounter++;
		byte block[CRI_BLOCK_SIZE];
		memcpy(block, &blocksBytes[i], CRI_BLOCK_SIZE);
		CRIBlock criBlock;
		criBlock.parse(block);
		blocks.push_back(criBlock);
	}
}

int CRI::size(){
	return blocks.size() * CRI_BLOCK_SIZE;
}

void CRI::search(fileID fid, CRIBlock& block){
	byte lowerFid[LOWERFID_SIZE];
	fid.getLowerID(lowerFid);
	search(lowerFid, block);
}

void CRI::search(byte lowerFid[], CRIBlock& block){
	for(int i = 0; i < blocks.size(); i++){
		if(blocks[i].match(lowerFid))
			block = blocks[i];
	}
}

bool CRI::isEmpty(){
	byte zeroLowerFid[LOWERFID_SIZE] = {0};
	if(blocks[0].match(zeroLowerFid))
		return true;
	else
		return false;
}
