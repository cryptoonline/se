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
#include "Cri.h"
#include "DataStructures.h"

using namespace std;

struct CRI{
	uint32_t prSubsetSeed;
	uint32_t prSubsetSize;
	unsigned char fid[32];
};

class OnlineSession {
    Communicator communicator;
	static DataStructures dataStructures;
    DataBlock** blocks;
	Cri cri;
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
//    unsigned char* fileDataRead;
   	unsigned char** goodCriBlocks;
 
    DataBlock** fileBlockstoBeWritten;
    unsigned char** encryptedfileBlockstoBeWritten;
 
	TBlock* tblock;   
    
   	/* T and D when reading them from local memory. */
   	char* T;
	char* D;
	
	/* DataBlock and TBlock object arrays for update*/
	DataBlock** D_session; 

	void readTRecord();
    bool readCRI();
    /*! Read file size from PRSubset and return it */
//    uint32_t getFileSize();
    /*! Read blocks represented by PRSubset */
    void getFile(vector<unsigned char>& fileContents);
//    bool searchCRI();
    int32_t search(unsigned char** arrayToSearchIn, unsigned char* arrayToSearchFor, int32_t rows, int32_t cols, int32_t startCol = 0, int32_t startRow = 0);
    
    void sendBlocks();
    void addBlockLocation();
    
    unsigned char* readT(uint32_t TRecordIndex);
    void writeT(uint32_t TRecordIndex,  unsigned char* block);
    
    unsigned char** readD(uint32_t* blockLocations, uint32_t numBlocks);
    void writeD(uint32_t* blockLocations, uint32_t numBlocks, unsigned char** blocks);

	void writeFile(unsigned char* fileBytes, uint32_t fileSize, fileID &fid, PRSubset &prSubset);
	void makeBlocks(unsigned char* fileBytes, uint32_t* prSubset, fileID &fid, uint32_t filesize);
	vector<uint32_t> getEmptyBlocks(PRSubset &prSubset);
	void writeFinalize(uint32_t numBlocks);
	
	void loadDataStructures();
	void loadFile(string filename, char* input, int64_t size); 
	int64_t readFileSize(string filename);
	   
public:
    OnlineSession(Communicator &communicator);
    ~OnlineSession();
    void get(string filename, int32_t size, vector<unsigned char>& fileContents);
    void update(unsigned char* input, uint32_t size, string filename);
    void remove();
    /*! Use homorphic encryption to support this */
    void rename();
};

#endif /* defined(__BlindStorage__Dfile__) */
