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
	//iv = new char[16]();
	//block = new unsigned char[T_BLOCK_SIZE];
	//this->encryptedBlock = encryptedBlock;
	memset(block, 0, T_BLOCK_SIZE);
	memcpy(this->encryptedBlock, encryptedBlock, T_BLOCK_SIZE);
	this->index = index;
	setupKey();
    parse();
}

void TBlock::set(uint32_t prSubsetSize, uint32_t prSubsetSeed, uint32_t index){
	//iv = new char[16];
	//encryptedBlock = new unsigned char[T_BLOCK_SIZE];
    this->prSubsetSize = prSubsetSize;
    this->prSubsetSeed = prSubsetSeed;
	this->index = index;
	memset(block, 0, T_BLOCK_SIZE);
	version = 0;
	setupKey();
   // block = new unsigned char[sizeof(this->prSubsetSize) + sizeof(this->prSubsetSeed) + sizeof(version)];
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
    memcpy(&block[4], static_cast<unsigned char*>(static_cast<void*>(&prSubsetSize)), sizeof(prSubsetSize));
	unsigned char* ciphertext = ENC();
	memcpy(encryptedBlock, ciphertext, T_BLOCK_SIZE - sizeof(version));
	delete[] ciphertext;
	memcpy(&block[8], static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(version));
	memcpy(&encryptedBlock[8], static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(version));
	cout << "Index in make is " << index << endl;
	//printhex(key, 16, "Key in make");
	//printhex(iv, 8, "IV in make");
	//printhex(block, T_BLOCK_SIZE, "Plaintext Block in make");
	//printhex(encryptedBlock, T_BLOCK_SIZE, "Encrypted Block in make");
}

void TBlock::parse(){
	version = *(uint32_t*)(encryptedBlock+(uint32_t)sizeof(prSubsetSeed)+(uint32_t)sizeof(prSubsetSize));
//	cout << "Version " << version << endl;
	cout << "Index in parse is " << index << endl;
	//printhex(key, 16, "Key in parse");
	//printhex(encryptedBlock, T_BLOCK_SIZE, "Encrypted Block in parse");
	unsigned char* plaintext = DEC();
	//printhex(iv, 8, "IV in parse");
	memcpy(block, plaintext, T_BLOCK_SIZE-sizeof(version));
	delete[] plaintext;
	memcpy(&block[T_BLOCK_SIZE-sizeof(version)], static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(version));
	//printhex(block, T_BLOCK_SIZE, "Decrypted Block in parse");
	prSubsetSize = *(uint32_t*)(block);
    prSubsetSeed = *(uint32_t*)(block+(uint32_t)sizeof(prSubsetSize));
}

unsigned char* TBlock::ENC(){
	Blowfish cipher;
	makeIV();
	if(key == NULL)
		cerr << "Key is NULL at line " << __LINE__ << " in " << __PRETTY_FUNCTION__ << endl;
	return cipher.ENC(block, T_BLOCK_SIZE - sizeof(version), reinterpret_cast<unsigned char*>(key), reinterpret_cast<unsigned char*>(iv)); 
}

unsigned char* TBlock::DEC(){
	Blowfish cipher;
	makeIV();
	return cipher.DEC(encryptedBlock, T_BLOCK_SIZE-sizeof(version) ,reinterpret_cast<unsigned char*>(key), reinterpret_cast<unsigned char*>(iv));
}

void TBlock::setupKey(){
	string keyFile = T_KEYFILE;
	if(!wasKeyGenerated){
		cout << "Key Generated" << endl;
		Key key(keyFile);
		TBlock::key = key.get();
		printhex(TBlock::key, 16, "TBlock key");
		wasKeyGenerated = true;
	}
//	cout << "Key is ";
//	for(int i = 0; i < 16; i++)
//		cout << (unsigned int) key[i];
//	cout << endl;
}

void TBlock::makeIV(){
//	memset(iv, 0, 8);
//	printf("Index is %x\n", index);
	memcpy(iv, static_cast<unsigned char*>(static_cast<void*>(&index)), 4);
	memcpy(iv+4, static_cast<unsigned char*>(static_cast<void*>(&version)), 4);
//	printhex(iv, 8, "IV");
//	cout << "IV is ";
//	for(int i = 0; i < 16; i++)
//		printf("%02X ", iv[i]);
//	cout << endl;
	
}

void TBlock::encryptIfEmpty(){
	if(!isOccupied()){
		unsigned char* ciphertext = ENC();
		memcpy(encryptedBlock, ciphertext, T_BLOCK_SIZE);
	}
}

bool TBlock::isOccupied(){
	return prSubsetSize;
}
