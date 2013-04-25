//
//  Ddisk.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__Ddisk__
#define __BlindStorage__Ddisk__

#include <iostream>
#include "Communicator.h"
#include "PRSubset.h"
#include "fileID.h"
#include "math.h"
#include <fstream>

using namespace std;
class Ddisk {
private:
    Communicator communicator;
    DataBlock** D; /*!< Array of DataBlocks i.e. D */
    
    char* readFileBytes(std::ifstream& file);
    void makeBlocks(unsigned char* fileBytes, uint32_t* prSusbset, fileID &fid, size_t filesize);
    vector<uint32_t> getEmptyBlocks(PRSubset &prSubset);
    
    
public:
    Ddisk(Communicator communicator);
    void addFile(std::ifstream& istream, fileID &fid, PRSubset &prSubset);
    void addBlocks(unsigned char* bytes, size_t size, fileID &fid, PRSubset &prSubset);
    void addBlock(uint64_t blockIndex, fileID &fid, unsigned char* rawData, uint32_t size);
    void finalize();
    void upload();
    void print(string tag, unsigned char* value, uint32_t size);
    void print(string tag, char* value, uint32_t size);
    void print(string tag, uint32_t* subset, uint32_t size);
};

#endif /* defined(__BlindStorage__Ddisk__) */
