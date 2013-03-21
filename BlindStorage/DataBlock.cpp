//
//  DataBlock.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "DataBlock.h"
#include "AES.h"

/**
 * \brief DataBlock is a data structure to hold the data blocks for Blind Storage System.
 * DataBlock has the following format
 **/

uint32_t DataBlock::counter = 0; /*!< Initializing the static variable */
bool DataBlock::wasKeyGenerated = false; /*!< Checking if key was generated and not generate it again */
unsigned char* DataBlock::key = new unsigned char[16];

/**
 DataBlock constructor.
 */
DataBlock::DataBlock(){
    blockIndex = counter++;
    version = -1; /** Initialized to -1 as it is incremented before its first use */
    size = BLOCKSIZE + 1 + 2 * sizeof(uint32_t);
    block = new unsigned char[size];
    iv = new unsigned char[16];
//    key = new unsigned char[16];
    if(!wasKeyGenerated){
        generateKey();
        wasKeyGenerated = true;
    }
}

/**
 DataBlock destructor.
 */
DataBlock::~DataBlock(){
    delete[] rawData;
    delete[] key;
    delete[] iv;
}
    
/*!<
 This function can be used to retrieve the decrypted
 */
unsigned char* DataBlock::get(){
    return DEC(block, size-(uint32_t)sizeof(version));
}
    
/*!<
 This functions accepts data size as input to handle the case when the data is less than the BLOCKSIZE.
 */
void DataBlock::set(fileID_t fid, unsigned char* rawData, uint32_t size){
    this->rawData = rawData;
    if(size < BLOCKSIZE){
        this->rawDataSize = size;
        this->fid = fid;
        padded = 0; /// 0 means that block will be padded and pad bit will be added followed by zeros
        ++version;
        makeBlock();
    }
    else if (size == BLOCKSIZE){
        this->rawDataSize = BLOCKSIZE;
        padded = 1; /// 1 means that pad bit will be added in the end if the block doesn't need padding
        this->fid = fid;
        ++version;
        makeBlock();
    }
    else{
        cout << "Size of the block is large than acceptable block size.";
    }
}

/*!< Create complete block of the following format ENC(rawData||paddedbit||fid)||version */
void DataBlock::makeBlock(){
    uint32_t pointer = 0;
        
    memcpy(block, rawData, rawDataSize);
    padBlock();
    pointer += BLOCKSIZE;
    
    block[pointer] = padded;
    pointer += 1;
        
    memcpy(&block[pointer], static_cast<unsigned char*>(static_cast<void*>(&fid)), sizeof(fid));
    pointer += (uint32_t)sizeof(fid);
    
    printBytes("Unencrypted", block, size);
        
    byte* encryptedBlock = ENC(block,size- (uint32_t)sizeof(version));
    memcpy(block, encryptedBlock,size-(uint32_t)sizeof(version));
    delete[] encryptedBlock;
    
    printBytes("Encrypted without version", block, size);
    memcpy(&block[pointer], static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(version));
    printBytes("Encrypted with Version", block, size);
}

/*!< 
 Use the pad the block when its less than required number of bytes 
 */
void DataBlock::padBlock(){
    if(!padded){
        block[rawDataSize] = 1;
        for(int i = rawDataSize + 1; i < BLOCKSIZE; i++)
            block[i] = 0;
    }
}

/*!< 
 This function encrypts the block. It uses makeIV() function to create IV and AES.ENC to encrypt the block 
 */
unsigned char* DataBlock::ENC(unsigned char* plaintext, uint32_t size){
    AES* cipher = new AES();
    makeIV(); //It will populate the iv parameter of the object
    unsigned char * ciphertext = NULL;
    ciphertext = cipher->ENC(plaintext, size, DataBlock::key, DataBlock::iv);
    delete cipher;
    return ciphertext;
}

/*!< 
 This function decrypts the block. It uses makeIV() function to create IV and AES.DEC to encrypt the block 
 */
unsigned char* DataBlock::DEC(unsigned char* ciphertext, uint32_t size){
    AES* cipher = new AES();
    makeIV(); //It will populate the iv parameter of the object
    unsigned char * plaintext = cipher->DEC(ciphertext, size, DataBlock::key, DataBlock::iv);
    delete cipher;
    return plaintext;
}

/*!< 
 This function is used to generate a random key using AES.keygen.
 */
void DataBlock::generateKey(){
    AES* cipher = new AES();
    DataBlock::key = cipher->keyGen();
    delete cipher;
}

/*!<
 This function can be used to set the key, if the key is already available.
 */
void DataBlock::setKey(unsigned char* key){
    DataBlock::key = key;
}

/*!<
 This function is used to create IV using blockIndex and version. The format of the IV is as:
 MSB --- LSBofblockIndex - MSBofblockIndex, LSBofversion-MSBofversion, 
 */
void DataBlock::makeIV(){
    memcpy(iv, static_cast<unsigned char*>(static_cast<void*>(&blockIndex)), sizeof(blockIndex));
    memcpy(iv+4, static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(version));
    unsigned char zeros[8] = {0};
    memcpy(iv+8, zeros, 8);
}

void DataBlock::print(string tag, unsigned char* value){
    cout << "<" << tag << ">" << value << "</" << tag << ">" << endl;
}

void DataBlock::print(string tag, int value){
    cout << "<" << tag << ">" << value << "</" << tag << ">" << endl;
}

void DataBlock::printBytes(string tag, int integer){
    unsigned char bytes[sizeof integer];
    std::copy(static_cast<const unsigned char*>(static_cast<const void*>(&integer)),
              static_cast<const unsigned char*>(static_cast<const void*>(&integer)) + sizeof integer,
              bytes);
    
    cout << endl << "*********************" << tag << " (start)*********************" << endl;
    for( int i = 0; i < sizeof integer; i++)
        cout << "[" << i << "] --> " << (uint8_t)bytes[i] << '\t';
    cout << endl << "**********************" << tag << " (end)**********************" << endl;
}

void DataBlock::printBytes(string tag, unsigned char* value, uint32_t size){
    cout << endl << "*********************" << tag << " (start)*********************" << endl;
    for( int i = 0; i < size; i++)
        cout << i << " --> " << (int)value[i] << '\t';
    cout << endl << "**********************" << tag << " (end)**********************" << endl;
}