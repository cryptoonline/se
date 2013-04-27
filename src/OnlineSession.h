//
//  Dfile.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__Dfile__
#define __BlindStorage__Dfile__

#include <stdint.h>
#include <iostream>
#include "Communicator.h"
#include "PRSubset.h"
#include "Ddisk.h"
#include "FFX.h"
#include "TBlock.h"

using namespace std;

class OnlineSession {
    Communicator communicator;
    DataBlock** blocks;
    PRSubset criPrSubset;
    PRSubset* filePrSubset;
    fileID criFid;
    unsigned char* fileCompleteID;
    fileID fileFid;
    string filename;
    
    unsigned char** decryptedCriFile;
    DataBlock** criFileBlocks;
    unsigned char** criEntries;
    DataBlock** fileBlocksRead;
    DataBlock** extractedFileBlocks;
    unsigned char** decryptedFileBlocksRead;
    unsigned char* fileDataRead;
    
    DataBlock** fileBlockstoBeWritten;
    unsigned char** encryptedfileBlockstoBeWritten;
    
    
    
    
    /*! Uses higher fid and prSubset to get get this */
    void getCRI();
    /*! Parses CRI to get lower fid and PRSubset for the actual file, PRSubset and all other required things are remembered */
    bool parseCRI();
    /*! Read file size from PRSubset and return it */
//    uint32_t getFileSize();
    /*! Read blocks represented by PRSubset */
    void getFile();
//    bool searchCRI();
    int32_t search(unsigned char** arrayToSearchIn, unsigned char* arrayToSearchFor, int32_t rows, int32_t cols, int32_t startCol = 0, int32_t startRow = 0);
    
    void sendBlocks();
    void addBlockLocation();
    
    unsigned char* readT(uint32_t TRecordIndex);
    void writeT(uint32_t TRecordIndex, unsigned char* block);
    
    unsigned char** readD(uint32_t* blockLocations, fileID &fid);
    void writeD(uint32_t* blockLocations, unsigned char** blocks);
    
public:
    OnlineSession(Communicator &communicator, PRSubset &criPrSubset, fileID &criFid, string filename);
    ~OnlineSession();
    unsigned char* get();
    void put(unsigned char* input, uint32_t size, string filename, fileID &fid, PRSubset &prSubset);
    void update(unsigned char* updatedInput, uint32_t size);
    void remove();
    /*! Use homorphic encryption to support this */
    void rename();
};

#endif /* defined(__BlindStorage__Dfile__) */
