//
//  OnlineSession.h
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
#include "PRF.h"
#include "TBlock.h"
#include "Tdisk.h"
#include "Debug.h"

using namespace std;

class OnlineSession {
    Communicator communicator;
    DataBlock** blocks;
    PRSubset* filePrSubset;
	PRSubset* criPrSubset;
    unsigned char* fileCompleteID;
    fileID fid;
    string filename;
    
    unsigned char** decryptedCriFile;
    DataBlock** criFileBlocks;
    unsigned char** criEntries;
    DataBlock** extractedFileBlocks;
    unsigned char** decryptedFileBlocksRead;
    unsigned char* fileDataRead;
   	unsigned char** goodCriBlocks;
 
    DataBlock** fileBlockstoBeWritten;
    unsigned char** encryptedfileBlockstoBeWritten;
 
	TBlock* tblock;   
    
   	/* T and D when reading them from local memory. */
   	char* T;
	char* D;
	
	/* DataBlock and TBlock object arrays for update*/
	DataBlock** D_session; 

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
    void writeT(uint32_t TRecordIndex,  unsigned char* block);
    
    unsigned char** readD(uint32_t* blockLocations, uint32_t numBlocks);
    void writeD(uint32_t* blockLocations, uint32_t numBlocks, unsigned char** blocks);

	void writeFile(unsigned char* fileBytes, uint32_t fileSize, fileID &fid, PRSubset &prSubset);
	void makeBlocks(unsigned char* fileBytes, uint32_t* prSubset, fileID &fid, size_t filesize);
	vector<uint32_t> getEmptyBlocks(PRSubset &prSubset);
	void writeFinalize(uint32_t numBlocks);
	
	void loadDataStructures();
	void loadFile(string filename, char* input, int64_t size); 
	int64_t readFileSize(string filename);
	   
public:
    OnlineSession(Communicator &communicator);
    ~OnlineSession();
    unsigned char* get(string filename, PRSubset* prSubset = NULL);
    void update(unsigned char* input, uint32_t size, string filename);
    void remove();
    /*! Use homorphic encryption to support this */
    void rename();
};

#endif /* defined(__BlindStorage__Dfile__) */
