//
// Cri.cpp
// BlindStorage
//

#include "Cri.h"

CRI::CRI(higherfid_t higherfid){
	this->higherfid = higherfid;
}

CRI::~CRI(){
}

void CRI::addFile(prSubsetSize_t size, prSubsetSeed_t seed, byte lowerFid[]){
	CRIBlock block;
	block.make(size, seed, lowerFid);
	blocks.push_back(block);
}

void CRI::makeBytes(vector<byte> blocksBytes){
	for(vector<CRIBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it){
		vector<byte> block;
		it->get(block);
		blocksBytes.insert(blocksBytes.end(), block.begin(), block.end());
	}
}

void CRI::parseBytes(vector<byte> blocksBytes){
	for(int i = 0; i < blocksBytes.size(); i+=CRI_BLOCK_SIZE){
		byte block[CRI_BLOCK_SIZE];
		std::copy(blocksBytes.begin()+i*CRI_BLOCK_SIZE, blocksBytes.begin()+i*CRI_BLOCK_SIZE+CRI_BLOCK_SIZE, block);
		CRIBlock criBlock;
		criBlock.parse(block);
		blocks.push_back(criBlock);
	}
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
