//
//  Dfile.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "OnlineSession.h"

OnlineSession::OnlineSession(Communicator &communicator, PRSubset &criPrSubset, fileID &criFid, string filename){
    this->communicator = communicator;
    this->criPrSubset = criPrSubset;
    this->criFid = criFid;
    this->filename = filename;
    this->fileCompleteID = fileFid.get();
}

unsigned char* OnlineSession::get(){
    getCRI();
    parseCRI();
    getFile();
    return fileData;
}

void OnlineSession::put(istream iStream){
    
}

void OnlineSession::update(istream updatedStream){
    
}

void OnlineSession::remove(){
    
}

void OnlineSession::rename(){
    
}

void OnlineSession::getCRI(){
//    uint32_t TRecordIndex = criFid.getPRPofHigherID();
//    unsigned char* block = readT(TRecordIndex);
//    TBlock tblock(block);
//
//    criPrSubset = new PRSubset(tblock.getPrSubsetSize(), tblock.getPrSubsetSeed());
    uint32_t* blockLocations = criPrSubset.get();
    
    unsigned char** criFile = readD(blockLocations, criFid);
    int32_t criNumBlocks = criPrSubset.getSize();
    
    criFileBlocks = new DataBlock*[criNumBlocks];
    for(int i = 0; i < criNumBlocks; i++){
        criFileBlocks[i] = new DataBlock(blockLocations[i], criFile[i*BLOCK_SIZE]);
        decryptedCriFile[i] = criFileBlocks[i]->getDecrypted();
    }
}

bool OnlineSession::parseCRI(){
    int criFileBytes = criPrSubset.getSize() / BLOW_UP * MAX_BLOCK_DATA_SIZE;
    criEntries = new unsigned char*[criFileBytes / 40]; //make 40 a parameter in parameters.h
    for(int i = 0; i < criFileBytes / 40; i++){
        criEntries[i] = new unsigned char[40];
        if(i % MAX_BLOCK_DATA_SIZE == 0)
            i += BLOCK_SIZE - MAX_BLOCK_DATA_SIZE;
        memcpy(criEntries[i], decryptedCriFile[i*40], 40);
    }

    int32_t match = search(criEntries, fileCompleteID, criPrSubset.getSize() / BLOW_UP, 40, 8, 0);
    if(match != -1){
//        unsigned char ID[32];
//        memcpy(ID,&decryptedCriFile[match][8], 40);
        filePrSubset = new PRSubset(*(uint32_t*)(criEntries[match]), *(uint32_t*)(criEntries[match]+4));
//        fileID fileFid(criEntries[match]+8);
//        this->fileFid = fileFid;
        return true;
    }
    
    return false; //return false if file not found.
}
    

//uint32_t OnlineSession::getFileSize(){
//    return filePrSubset->getSize();
//}

void OnlineSession::getFile(){
    uint32_t* blockLocations = filePrSubset->get();
    unsigned char** encryptedBlocksData = readD(blockLocations, fileFid);
    int32_t prSubsetSize = filePrSubset->getSize();
    decryptedBlocks = new unsigned char*[prSubsetSize];
    fileData = new unsigned char[prSubsetSize/BLOW_UP];

    fileBlocks = new DataBlock*[prSubsetSize];
    for(int i = 0; i < prSubsetSize; i++){
        fileBlocks[i] = new DataBlock(blockLocations[i], encryptedBlocksData[i]);
        decryptedBlocks[i] = new unsigned char[BLOCK_SIZE]();
        memcpy(decryptedBlocks[i], fileBlocks[i]->getDecrypted(), BLOCK_SIZE);
        if(fileBlocks[i]->checkFileID(fileFid))
            memcpy(&fileData[i*MAX_BLOCK_DATA_SIZE], decryptedBlocks[i], MAX_BLOCK_DATA_SIZE);
    }
}

unsigned char* OnlineSession::readT(uint32_t TRecordIndex){
    
}

void OnlineSession::writeT(uint32_t TRecordIndex, unsigned char* block){
    
}

unsigned char** OnlineSession::readD(uint32_t* blockLocations, fileID &fid){
    // The function should return just the blocks that correspond to correct blocks.
    
}

void OnlineSession::writeD(uint32_t* blockLocations, unsigned char** blocks){
    
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






