//
//  DataBlock.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "DataBlock.h"

byte DataBlock::key[AES_KEY_SIZE];
b_index_t DataBlock::instanceCounter = 0;

DataBlock::DataBlock(){
	instanceCounter++;
	index = instanceCounter;
}

DataBlock::DataBlock(b_index_t index){
	this->index = index;
}

DataBlock::~DataBlock(){
}

void DataBlock::setKey(byte key[]){
	memcpy(this->key, key, AES_KEY_SIZE);
}

void DataBlock::make(fileID fid, byte block[], int dataSize, bool isCRI, version_t version) {
	this->fid = fid;
	this->isCRI = isCRI;
	this->version = version;
	this->block = block;
	this->dataSize = dataSize;
	addTrailer();
	encrypt();
}

void DataBlock::addTrailer(){
	if(dataSize < MAX_BLOCK_DATA_SIZE)
		addPadding();
	else
		block[PADBYTE_LOC] = 1;

	block[CRIBYTE_LOC] = isCRI;
//	memcpy(&block[FILEID_LOC], fid.get(), FILEID_SIZE);
	memcpy(&block[VERSION_LOC], static_cast<byte*>(static_cast<void*>(&version)), sizeof(version_t));
}

void DataBlock::addPadding(){
	block[dataSize] = 0;
}

void DataBlock::removePadding(){
	for(int i = MAX_BLOCK_DATA_SIZE; i > 0; i--)
		if(block[i] == 1){
			dataSize = i-1;
		}
}

void DataBlock::encrypt(){
	AES cipher;
	makeIV();
	cipher.ENC_CTR(block, block, BLOCK_SIZE-sizeof(version_t), key, iv);
	isBlockEncrypted = true;
}

void DataBlock::decrypt(){
	AES cipher;
	makeIV();
	cipher.DEC_CTR(block, block, BLOCK_SIZE-sizeof(version_t), key, iv);
	isBlockEncrypted = false;
}

void DataBlock::makeIV(){
	memcpy(iv, static_cast<byte*>(static_cast<void*>(&index)), sizeof(b_index_t));
	memcpy(iv+sizeof(b_index_t), static_cast<byte*>(static_cast<void*>(&version)), sizeof(version_t));
	memset(iv+sizeof(b_index_t)+sizeof(version_t), 0, AES_BLOCK_SIZE-sizeof(b_index_t)+sizeof(version_t));
}

int DataBlock::getDataSize(){
	return dataSize;
}
