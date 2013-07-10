//
// CRIBlock.cpp
// BlindStorage
//

#include "CRIBlock.h"

CRIBlock::CRIBlock(){
	size = 0;
	seed = 0;
	memset(lowerFid, 0, LOWERFID_SIZE);
	memset(block, 0, CRI_BLOCK_SIZE);
}

CRIBlock::~CRIBlock(){
}

void CRIBlock::make(prSubsetSize_t size, prSubsetSeed_t seed, byte lowerFid[]){
	this->size = size;
	this->seed = seed;
	memcpy(this->lowerFid, lowerFid, LOWERFID_SIZE);	
	memcpy(block, static_cast<byte*>(static_cast<void*>(&size)), sizeof(prSubsetSize_t));
	memcpy(block+sizeof(prSubsetSize_t), static_cast<byte*>(static_cast<void*>(&seed)), sizeof(prSubsetSeed_t));
	memcpy(block+sizeof(prSubsetSize_t)+sizeof(prSubsetSeed_t), lowerFid, LOWERFID_SIZE);
}

void CRIBlock::update(prSubsetSize_t size, prSubsetSeed_t seed){
	this->size = size;
	this->seed = seed;
	memcpy(block, static_cast<byte*>(static_cast<void*>(&size)), sizeof(prSubsetSize_t));
	memcpy(block+sizeof(prSubsetSize_t), static_cast<byte*>(static_cast<void*>(&seed)), sizeof(prSubsetSeed_t));
}

void CRIBlock::parse(byte block[]){
	memcpy(this->block, block, CRI_BLOCK_SIZE);
	size = *(prSubsetSize_t*)(block);
	seed = *(prSubsetSeed_t*)(block+sizeof(prSubsetSize_t));
	memcpy(lowerFid, block+sizeof(prSubsetSize_t)+sizeof(prSubsetSeed_t), LOWERFID_SIZE);
}

void CRIBlock::get(byte block[]){
	memcpy(block, this->block, CRI_BLOCK_SIZE);
}

void CRIBlock::get(vector<byte>& block){
	block.insert(block.begin(), &(this->block[0]), &(this->block[CRI_BLOCK_SIZE]));
}

bool CRIBlock::match(byte lowerFid[]){
	return !memcmp(lowerFid, this->lowerFid, LOWERFID_SIZE);
}

prSubsetSize_t CRIBlock::getSize(){
	return size;
}

prSubsetSeed_t CRIBlock::getSeed(){
	return seed;
}
