//
// TBlock.cpp
// BlindStorage
//

#include "TBlock.h"

byte TBlock::key[AES_KEY_SIZE] = {0};
t_index_t TBlock::instanceCounter = 0;

TBlock::TBlock(){
	version = 0;
	size = 0;
	seed = 0;
	index = instanceCounter;
	cout << "Index is " << index;
	instanceCounter++;
	isBlockEncrypted = false;
}

TBlock::TBlock(t_index_t index){
	version = 0;
	size = 0;
	seed = 0;
	this->index = index;
	isBlockEncrypted = false;
}

TBlock::~TBlock(){
}

void TBlock::setKey(byte key[]){
	memcpy(this->key, key, AES_KEY_SIZE);
}

void TBlock::make(prSubsetSize_t size, prSubsetSeed_t seed){
	this->size = size;
	this->seed = seed;

	memcpy(block, static_cast<byte*>(static_cast<void*>(&size)), sizeof(prSubsetSize_t));
	memcpy(block+sizeof(prSubsetSize_t), static_cast<byte*>(static_cast<void*>(&seed)), sizeof(prSubsetSeed_t));
	memcpy(block+sizeof(prSubsetSize_t)+sizeof(prSubsetSeed_t), static_cast<byte*>(static_cast<void*>(&version)), sizeof(version_t));

	encrypt();
}

void TBlock::parse(byte block[]){
//	this->block = block;
	memcpy(this->block, block, TBLOCK_SIZE);
	version = *(version_t*)(block+sizeof(prSubsetSize_t)+sizeof(prSubsetSeed_t));
	isBlockEncrypted = true;
	decrypt();
	size = *(prSubsetSize_t*)(this->block);
	seed = *(prSubsetSeed_t*)(this->block+sizeof(prSubsetSize_t));
}

void TBlock::update(prSubsetSize_t size, prSubsetSeed_t seed){
	version++;
	make(size, seed);
}

void TBlock::encrypt(){
	TruncAES cipher;
	makeIV();
	cipher.ENC(block, block, TBLOCK_SIZE-sizeof(version_t), key, iv);
	isBlockEncrypted = true;
}

void TBlock::decrypt(){
	TruncAES cipher;
	makeIV();
	cipher.DEC(block, block, TBLOCK_SIZE-sizeof(version_t), key, iv);
	isBlockEncrypted = false;
}

void TBlock::makeIV(){
	memcpy(iv, static_cast<byte*>(static_cast<void*>(&index)), sizeof(t_index_t));
	memcpy(iv+sizeof(t_index_t), static_cast<byte*>(static_cast<void*>(&version)), sizeof(version_t));
	memset(iv+sizeof(t_index_t)+sizeof(version_t), 0, AES_BLOCK_SIZE-sizeof(t_index_t)-sizeof(version_t));
}

void TBlock::getEncrypted(byte block[]){
	if(!isBlockEncrypted)
		encrypt();
	memcpy(block, this->block, TBLOCK_SIZE);
}

void TBlock::getDecrypted(byte block[]){
	if(isBlockEncrypted)
		decrypt();
	memcpy(block, this->block, TBLOCK_SIZE);
}

prSubsetSize_t TBlock::getSize(){
	return size;
}

prSubsetSeed_t TBlock::getSeed(){
	return seed;
}

bool TBlock::isOccupied(){
	return size ? true : false;	
}

void TBlock::encryptIfEmpty(){
	if(!isOccupied()){
		memset(block, 0, TBLOCK_SIZE);
		encrypt();
	}
}
