//
//  Ddisk.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__Ddisk__
#define __BlindStorage__Ddisk__

#include <stdint.h>
#include <iostream>
#include "Communicator.h"
#include "PRSubset.h"
#include "fileID.h"
#include "math.h"
#include <fstream>
#include "DataBlock.h"
#include "Debug.h"

class Ddisk {
private:
    Communicator communicator;
    DataBlock** D; /*!< Array of DataBlocks i.e. D */
    uint32_t numBlocks;
    
    char* readFileBytes(string filename, size_t size);
	size_t readFileSize(string filename);
    void makeBlocks(unsigned char* fileBytes, uint32_t* prSusbset, fileID &fid, size_t filesize);
    vector<uint32_t> getEmptyBlocks(PRSubset &prSubset);
    
    
public:
	Ddisk();
    Ddisk(Communicator communicator);
    Ddisk(uint32_t numBlocks);
    void addFile(string filename);
	void addFile(string filename, PRSubset& prSubset);
    void addFile(string filename, unsigned char* fileBytes, uint32_t fileSize, PRSubset &prSubset);
    void addBlocks(unsigned char* bytes, size_t size, fileID &fid, PRSubset &prSubset);
    void addBlock(uint64_t blockIndex, fileID &fid, unsigned char* rawData, uint32_t size);
    void finalize();
    void upload();
	void writeToDisk();
    DataBlock** get();
    void print(string tag, unsigned char* value, uint32_t size);
    void print(string tag, char* value, uint32_t size);
    void print(string tag, uint32_t* subset, uint32_t size);
};

#endif /* defined(__BlindStorage__Ddisk__) */
