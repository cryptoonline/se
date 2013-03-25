//
//  DataBlock.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__DataBlock__
#define __BlindStorage__DataBlock__

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <fstream>

#include "AES.h"
#include "parameters.h"

typedef uint32_t fileID_t; /*!< \typedef File ID type can be specified here */

class DataBlock{
public:
    DataBlock();
    DataBlock(uint64_t blockIndex, fileID_t fid, unsigned char* rawData, uint32_t size);
    DataBlock(uint64_t blockIndex);
    ~DataBlock();
    
    unsigned char* getDecrypted();
    unsigned char* getEncrypted();
    void update(uint64_t blockIndex, fileID_t fid, unsigned char* rawData, uint32_t size);
    void generateKey();
    void setKey(unsigned char* key);
    unsigned char* ENC();
    bool isOccupied();

    void print(string tag, unsigned char* value);
    void print(string tag, int value);
    void printBytes(string tag, int integer);
    void printBytes(string tag, unsigned char* value, uint32_t size);
    void printBytes(string tag, char* value, uint32_t size);

        
private:
    static unsigned char* key; /*!< The key is used for encrypting the block. Single key is used to encrypt all the blocks */
    static bool wasKeyGenerated; /*!< Flag to see if key is already generated */
    unsigned char* iv; /*!< The iv is used as initialization vector to encrypt blocks. makeIV method generates unique IV for every block. */
    uint32_t version; /*!< Version of the block, version is updated every time block is created. Maximum number of time block can be updates is 2^32  */
    static uint32_t counter; /*!< Counter is static so it can be used to increment blockIndex for each new DataBlock created */
    uint64_t blockIndex; /*!< Index of the block, it's incremented each time new datablock object is created. Maximum number of blocks can be 2^64*/
    unsigned char* rawData; /*!< File data block, rawData in this class is pointer and will modify the data that is used in set()*/
    uint32_t rawDataSize; /*!< This represets the size of the data in the block and it is always equal to the BLOCKSIZE except the last block of the file, where it is less than BLOCKSIZE and block is padded. */
    uint32_t size; /*!< This repesents the size of the block, maximum size of the block can be 2^32 as limited by the method used for creating unique IV for every block. */
    unsigned char padded; /*!< This byte shows wether block is padded or not, 0 not padded, 1 padded*/
    uint32_t fid; /*!< File id */
    unsigned char* block; /*!< This is used to hold the block data, padding byte,file id and version. All fields but version are encrytpted. Version is used in IV creation and there can't be encrypted */
    const char* keyFilename;
    void saveKeytoFile();
    void loadKeyfromFile();
    bool isKeyFileStored();
    void setupKey();
    
    void makeBlock();
    void padBlock();
    void makeIV();
    void add(uint64_t blockIndex, fileID_t fid, unsigned char* rawData, uint32_t size);
    
    
    unsigned char* DEC();

};



#endif /* defined(__BlindStorage__DataBlock__) */
