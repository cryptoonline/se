//
//  Ddisk.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "Ddisk.h"

void Ddisk::addBlocks(unsigned char* fileBytes, size_t filesize, fileID &fid, PRSubset &prSubset){
    vector<uint32_t> emptyBlocks = getEmptyBlocks(prSubset);
	if(emptyBlocks.size()*MAX_BLOCK_DATA_SIZE < filesize){
		cerr << "Not enough empty blocks" << endl;
        exit(1);
    }
    makeBlocks(fileBytes, emptyBlocks.data(), fid, filesize);
    
//    makeBlocks(fileBytes, prSubset.get(), fid, filesize);
}

void Ddisk::makeBlocks(unsigned char* fileBytes, uint32_t* prSubset, fileID &fid, size_t filesize){
    int counter = 0;
    int32_t numBlocks = (int32_t)ceil((double)filesize/(double)MAX_BLOCK_DATA_SIZE);
    unsigned char* blocks = new unsigned char[numBlocks*BLOCK_SIZE]();
    
    for(; counter < numBlocks - 1; counter++){
        memcpy(&blocks[counter * BLOCK_SIZE], &fileBytes[counter*MAX_BLOCK_DATA_SIZE] , MAX_BLOCK_DATA_SIZE);
        D[prSubset[counter]] = new DataBlock(prSubset[counter], fid, &blocks[counter * BLOCK_SIZE], MAX_BLOCK_DATA_SIZE);
    }

    uint32_t sizeOfLastBlock = (uint32_t)filesize - ((uint32_t)filesize/MAX_BLOCK_DATA_SIZE) * MAX_BLOCK_DATA_SIZE;
    memcpy(&blocks[counter * BLOCK_SIZE], &fileBytes[counter*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
    D[prSubset[counter]] = new DataBlock(prSubset[counter], fid, &blocks[counter * BLOCK_SIZE], sizeOfLastBlock);
}

char* Ddisk::readFileBytes(string filename, size_t size){
	ifstream file(filename.c_str(), std::ios::binary);
    char* bytes = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(bytes, size);
    file.close();
    return bytes;
}

size_t Ddisk::readFileSize(string filename){	
    ifstream file(filename.c_str());
	file.seekg(0, std::ios::end);
    size_t filesize = file.tellg();
	return filesize;
}

vector<uint32_t> Ddisk::getEmptyBlocks(PRSubset &prSubset){
    uint32_t prSubsetSize = prSubset.getSize();
    uint32_t* subset = prSubset.get();
    std::vector<uint32_t> emptyBlocks;
    for(int i = 0; i < prSubsetSize; i++){
        if(!D[subset[i]]->isOccupied())
            emptyBlocks.push_back(subset[i]);
    }
    
    return emptyBlocks;
}

void Ddisk::finalize(){
    for(int i = 0; i < numBlocks; i++)
        if(!D[i]->isOccupied()){
            D[i]->encryptIfEmpty();
        }
}

void Ddisk::upload(){
    
}

void Ddisk::writeToDisk(){
	ofstream file("data/D", std::ios_base::binary);
	for(int i = 0; i < numBlocks; i++){
		cout << i*100/numBlocks << "\% blocks processed";
		cout.flush();
		cout << "\r";
		file.write(reinterpret_cast<char*>(D[i]->getEncrypted()), BLOCK_SIZE);	
	}
	cout << endl;
}

DataBlock** Ddisk::get(){
    return D;
}
