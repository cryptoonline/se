//
//  OnlineSession.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "OnlineSession.h"

OnlineSession::OnlineSession(Communicator &communicator){
    this->communicator = communicator;
//    this->criPrSubset = criPrSubset;
//    this->criFid = criFid;
//    this->filename = filename;
//    this->fileCompleteID = fileFid.get();
}

unsigned char* OnlineSession::get(string filename, PRSubset* prSubset){
	fileID fid(filename);
	this->fid = fid;
	fileCompleteID = fid.get();

    getCRI();
    bool fileExists = parseCRI();
	if(!fileExists)
		filePrSubset = prSubset;
    getFile();
    return fileDataRead;
}

void OnlineSession::update(unsigned char* input, uint32_t size, string filename){
//    uint32_t numBlocks = (uint32_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);
    
	if(size > filePrSubset->getSize()){
		int seed = filePrSubset->getSeed();
		delete filePrSubset;	
		filePrSubset = new PRSubset(size, seed);
	}
    Ddisk D(filePrSubset->getSize());
    D.addFile(input, size, fid, *filePrSubset);
   	D.finalize();
    
	fileBlockstoBeWritten = D.get();
    for(int i = 0; i < filePrSubset->getSize(); i++)
        encryptedfileBlockstoBeWritten[i] = fileBlockstoBeWritten[i]->getEncrypted();
    if(tblock != NULL)
		tblock->update(filePrSubset->getSize(), filePrSubset->getSeed());
	else
		cerr << "Access NULL pointer  at line " << __LINE__ << " in " << __PRETTY_FUNCTION__; 
    writeT(fid.getPRPofHigherID(), tblock->getEncrypted());
    writeD(filePrSubset->get(), filePrSubset->getSize(), encryptedfileBlockstoBeWritten);
}

void OnlineSession::remove(){
//    get();
//    for(int i = 0; filePrSubset->getSize() / BLOW_UP; i++){
//        fileID zeroFid;
//        extractedFileBlocks[i]->update(zeroFid, NULL, 0);
//    }
//    Update other blocks also, just have to update their version and encrypt
    
//    writeT();
//    writeD(prSubset.get(),
    
}

void OnlineSession::rename(){
    
}

void OnlineSession::getCRI(){
    uint32_t TRecordIndex = fid.getPRPofHigherID();
    unsigned char* block = readT(TRecordIndex);
    tblock = new TBlock(block, TRecordIndex);

    criPrSubset = new PRSubset(tblock->getPrSubsetSize(), tblock->getPrSubsetSeed());
    uint32_t* blockLocations = criPrSubset->get();
    
    int32_t criNumBlocks = criPrSubset->getSize();
    unsigned char** criFile = readD(blockLocations, criNumBlocks);
    
    criFileBlocks = new DataBlock*[criNumBlocks];
    for(int i = 0; i < criNumBlocks; i++){
        criFileBlocks[i] = new DataBlock(blockLocations[i], criFile[i*BLOCK_SIZE]);
        decryptedCriFile[i] = criFileBlocks[i]->getDecrypted();
    }
}

bool OnlineSession::parseCRI(){
    int criFileBytes = criPrSubset->getSize() / BLOW_UP * MAX_BLOCK_DATA_SIZE;
    criEntries = new unsigned char*[criFileBytes / CRI_ENTRY_SIZE]; //make 40 a parameter in parameters.h
    for(int i = 0; i < criFileBytes / CRI_ENTRY_SIZE; i++){
        criEntries[i] = new unsigned char[CRI_ENTRY_SIZE];
        if(i % MAX_BLOCK_DATA_SIZE == 0)
            i += BLOCK_SIZE - MAX_BLOCK_DATA_SIZE;
        memcpy(criEntries[i], decryptedCriFile[i*CRI_ENTRY_SIZE], CRI_ENTRY_SIZE);
    }
//make it use checkFileID
    int32_t match = search(criEntries, fileCompleteID, criPrSubset->getSize() / BLOW_UP, CRI_ENTRY_SIZE, 8, 0);
    if(match != -1){
        filePrSubset = new PRSubset(*(uint32_t*)(criEntries[match]), *(uint32_t*)(criEntries[match]+4));
        return true;
    }
    
    return false; //return false if file not found.
}
    

//uint32_t OnlineSession::getFileSize(){
//    return filePrSubset->getSize();
//}

void OnlineSession::getFile(){
    uint32_t* blockLocations = filePrSubset->get();
    int32_t prSubsetSize = filePrSubset->getSize();
    unsigned char** encryptedBlocksData = readD(blockLocations, prSubsetSize);
    decryptedFileBlocksRead = new unsigned char*[prSubsetSize];
    fileDataRead = new unsigned char[prSubsetSize/BLOW_UP];

    fileBlocksRead = new DataBlock*[prSubsetSize];
    extractedFileBlocks = new DataBlock*[prSubsetSize/BLOW_UP];
    int j = 0;
    for(int i = 0; i < prSubsetSize; i++){
        fileBlocksRead[i] = new DataBlock(blockLocations[i], encryptedBlocksData[i]);
        decryptedFileBlocksRead[i] = new unsigned char[BLOCK_SIZE]();
        memcpy(decryptedFileBlocksRead[i], fileBlocksRead[i]->getDecrypted(), BLOCK_SIZE);
        if(fileBlocksRead[i]->checkFileID(fid)){
            extractedFileBlocks[j] = fileBlocksRead[i];
            memcpy(&fileDataRead[j*MAX_BLOCK_DATA_SIZE], decryptedFileBlocksRead[i], MAX_BLOCK_DATA_SIZE);
            j++;
        }
    }
}

unsigned char* OnlineSession::readT(uint32_t TRecordIndex){
	unsigned char* TEntry = new unsigned char[T_BLOCK_SIZE];
	communicator.tGet(TRecordIndex, reinterpret_cast<char*>(TEntry));
	return TEntry;
}

void OnlineSession::writeT(uint32_t TRecordIndex, unsigned char* block){
	communicator.tPut(TRecordIndex, reinterpret_cast<char*>(block));
}

unsigned char** OnlineSession::readD(uint32_t* blockLocations, uint32_t numBlocks){
	unsigned char** blocks = new unsigned char*[numBlocks];
	for(int i = 0; i < numBlocks; i++){
		communicator.dGet(blockLocations[i], reinterpret_cast<char*>(blocks[i]));
	}
	return blocks;
}

void OnlineSession::writeD(uint32_t* blockLocations, uint32_t numBlocks, unsigned char** blocks){
	for(int i = 0; i < numBlocks; i++){
		communicator.dPut(blockLocations[i], reinterpret_cast<char*>(blocks[i]));
	}
}
           
           
int32_t OnlineSession::search(unsigned char** arrayToSearchIn, unsigned char* arrayToSearchFor, int32_t rows, int32_t cols, int32_t startCol, int32_t startRow){
    int matches;
    for(int i = startRow; i < rows; i++){
        matches = 0;
        for(int j = startCol; j < cols; j++){
            if( arrayToSearchFor[j] == arrayToSearchIn[i][j])
                matches++;
            if(matches == 32)
                return i;
        }
    }
    return -1;
}

//bool OnlineSession::searchCRI(){
//    unsigned char* fileID = fileFid.get();
//    int matches = 0;
//    for(int k = 0; k < 32; k++){
//        for(int i = 0; i < criPrSubset.getSize(); i++){
//            for(int j = 0; j < MAX_BLOCK_DATA_SIZE; j++){
//                if(decryptedCriFile[i][j] == fileID[k]){
//                    k++;
//                    matches++;
//                    if(matches == 32)
//                        return true;
//                }
//                else
//                    matches = 0;
//            }
//        }
//    }
//    return false;
//}






