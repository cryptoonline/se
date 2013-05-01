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
#include "fileID.h"
#include "Key.h"

class DataBlock{
public:
    void initialize();
    DataBlock();
    DataBlock(uint32_t blockIndex, fileID &fid, unsigned char* rawData, uint32_t size);
    DataBlock(uint32_t blockIndex, unsigned char* block);
    ~DataBlock();
    
    unsigned char* getDecrypted();
    unsigned char* getEncrypted();
    void update(fileID &fid, unsigned char* rawData, uint32_t size);
    void deleteBlock();
    void generateKey();
    void setKey(unsigned char* key);
    const bool isOccupied();
    void add(uint32_t blockIndex, fileID &fid, unsigned char* rawData, uint32_t size);
    void encryptIfEmpty();
    bool checkFileID(fileID &fid);


    void print(string tag, unsigned char* value);
    void print(string tag, int value);
    void printBytes(string tag, int integer);
    void printBytes(string tag, unsigned char* value, uint32_t size);
    void printBytes(string tag, char* value, uint32_t size);


    
private:
    static unsigned char* key;      /*!< The key is used for encrypting the block. Single key is used to encrypt all the blocks */
    static bool wasKeyGenerated;    /*!< Flag to see if key is already generated */
    unsigned char* iv;              /*!< The iv is used as initialization vector to encrypt blocks. makeIV method generates unique IV for every block. */
    uint32_t version;               /*!< Version of the block, version is updated every time block is created. Maximum number of time block can be updates is 2^32  */
    uint32_t blockIndex;            /*!< Index of the block, it's incremented each time new datablock object is created. Maximum number of blocks can be 2^64*/
    unsigned char* rawData;         /*!< File data block, rawData in this class is pointer and will modify the data that is used in set()*/
    uint32_t rawDataSize;           /*!< This represets the size of the data in the block and it is always equal to the BLOCKSIZE except the last block of the file, where it is less than BLOCKSIZE and block is padded. */
    unsigned char padded;           /*!< This byte shows wether block is padded or not, 0 not padded, 1 padded*/
    fileID* fid;                   /*!< File id */
    uint32_t higherFid;
    unsigned char* block;           /*!< This is used to hold the block data, padding byte,file id and version. All fields but version are encrytpted. Version is used in IV creation and there can't be encrypted */
    static const char* keyFilename;
    void saveKeytoFile();
    void loadKeyfromFile();
    const bool isKeyFileStored();
    void setupKey();
    
    void makeBlock();
    void parseBlock();
    void addPadding();
    void removePadding();
    void makeIV();
    
    unsigned char* ENC();
    unsigned char* DEC();

};



#endif /* defined(__BlindStorage__DataBlock__) */
