//
//  TBlock.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "TBlock.h"

bool TBlock::wasKeyGenerated = false;
char* TBlock::key = NULL;

TBlock::TBlock(){
    prSubsetSeed = 0;
    prSubsetSize = 0;
}

TBlock::~TBlock(){
	delete[] iv;
}

TBlock::TBlock(unsigned char* encryptedBlock, uint32_t index){
	iv = new char[16];
	this->encryptedBlock = encryptedBlock;
	this->index = index;
	DEC();
	setupKey();
    parse();
}

void TBlock::set(uint32_t prSubsetSize, uint32_t prSubsetSeed, uint32_t index){
	iv = new char[16];
    this->prSubsetSize = prSubsetSize;
    this->prSubsetSeed = prSubsetSeed;
	this->index = index;
	version = 0;
	setupKey();
    block = new unsigned char[sizeof(this->prSubsetSize) + sizeof(this->prSubsetSeed) + sizeof(version)];
    make();
}

void TBlock::update(uint32_t prSubsetSize, uint32_t prSubsetSeed){
	version++;
	this->prSubsetSize = prSubsetSize;
	this->prSubsetSeed = prSubsetSeed;
	make();		
}

unsigned char* TBlock::getDecrypted(){
    return block;
}

unsigned char* TBlock::getEncrypted(){
	return encryptedBlock;
}

uint32_t TBlock::getPrSubsetSeed(){
    return prSubsetSeed;
}

uint32_t TBlock::getPrSubsetSize(){
    return prSubsetSize;
}

void TBlock::make(){
    memcpy(block, static_cast<unsigned char*>(static_cast<void*>(&prSubsetSeed)), sizeof(prSubsetSeed));
    memcpy(block, static_cast<unsigned char*>(static_cast<void*>(&prSubsetSize)), sizeof(prSubsetSize));
	unsigned char* ciphertext = ENC();
	memcpy(block, ciphertext, T_BLOCK_SIZE);
	delete[] ciphertext;
	memcpy(block, static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(version));
}

void TBlock::parse(){
    prSubsetSeed = *(uint32_t*)block;
    prSubsetSize = *(uint32_t*)block+(uint32_t)sizeof(prSubsetSeed);
	version = *(uint32_t*)block+(uint32_t)sizeof(prSubsetSeed)+(uint32_t)sizeof(prSubsetSize);
}

unsigned char* TBlock::ENC(){
	AES cipher;
	return cipher.ENC(block, T_BLOCK_SIZE - sizeof(version), reinterpret_cast<unsigned char*>(key), reinterpret_cast<unsigned char*>(iv)); 
}

unsigned char* TBlock::DEC(){
	AES cipher;
	return cipher.DEC(encryptedBlock, T_BLOCK_SIZE-sizeof(version) ,reinterpret_cast<unsigned char*>(key), reinterpret_cast<unsigned char*>(iv));
}

void TBlock::setupKey(){
	if(!wasKeyGenerated){
		Key key(T_KEYFILE);
		TBlock::key = key.get();
		wasKeyGenerated = true;
	}
}

void TBlock::makeIV(){
	memcpy(iv, static_cast<unsigned char*>(static_cast<void*>(&index)), 4);
	memcpy(iv+4, static_cast<unsigned char*>(static_cast<void*>(&version)), 4);
}
