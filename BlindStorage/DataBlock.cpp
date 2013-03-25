//
//  DataBlock.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "DataBlock.h"

/*!
 * \brief DataBlock is a data structure to hold the data blocks for Blind Storage System.
 * DataBlock has the following format
 **/

uint32_t DataBlock::counter = 0; /*!< Initializing the static variable */
bool DataBlock::wasKeyGenerated = false;
unsigned char* DataBlock::key = new unsigned char[16];

/*!
 DataBlock constructor.
 */
DataBlock::DataBlock(){
    version = -1; /*!< Initialized to -1 as it is incremented before its first use */
    size = BLOCKSIZE + 1 + 2 * sizeof(uint32_t);
    block = new unsigned char[size]();
    iv = new unsigned char[16];
    keyFilename = "secret.txt";
    setupKey();
}

/*!
 Constructor for the online phase, will return decrypted block
 */
DataBlock::DataBlock(uint64_t blockIndex){
    return block;
    
}

DataBlock::DataBlock(uint64_t blockIndex, fileID_t fid, unsigned char* rawData, uint32_t size){
    update(blockIndex, fid, rawData, size);
}

/*!
 DataBlock destructor.
 */
DataBlock::~DataBlock(){
    delete[] rawData;
    delete[] key;
    delete[] iv;
}
    
/*!
 This function can be used to retrieve the decrypted
 */
unsigned char* DataBlock::getEncrypted(){
    return DEC();
}

unsigned char* DataBlock::getDecrypted(){
    return block;
}
    
/*!
 This functions accepts data size as input to handle the case when the data is less than the BLOCKSIZE.
 */
void DataBlock::update(uint64_t blockIndex, fileID_t fid, unsigned char* rawData, uint32_t size){
    this->rawData = rawData;
    this->blockIndex = blockIndex;
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

void DataBlock::add(uint64_t blockIndex, fileID_t fid, unsigned char* rawData, uint32_t size){
    this->rawData = rawData;
    this->blockIndex = blockIndex;
    if(size < BLOCKSIZE){
        this->rawDataSize = size;
        this->fid = fid;
        padded = 0;
        makeBlock();
    }
    else if (size == BLOCKSIZE){
        this->rawDataSize = BLOCKSIZE;
        padded = 1;
        this->fid = fid;
        makeBlock();
    }
    else{
        cout << "Size of the block is large than the acceptable size.";
    }
    
}

/*! Create complete block of the following format ENC(rawData||paddedbit||fid)||version */
void DataBlock::makeBlock(){
    uint32_t pointer = 0;
        
    memcpy(block, rawData, rawDataSize);
    padBlock();
    pointer += BLOCKSIZE;
    
    block[pointer] = padded;
    pointer += 1;
        
    memcpy(&block[pointer], static_cast<unsigned char*>(static_cast<void*>(&fid)), sizeof(fid));
    pointer += (uint32_t)sizeof(fid);
    
//    printBytes("Unencrypted", block, size);
    
    byte* encryptedBlock = ENC();
    memcpy(block, encryptedBlock,size-(uint32_t)sizeof(version));
    delete[] encryptedBlock;
    
//    printBytes("Encrypted without version", block, size);
    memcpy(&block[pointer], static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(version));
//    printBytes("Encrypted with Version", block, size);
}

/*! 
 Use the pad the block when its less than required number of bytes 
 */
void DataBlock::padBlock(){
    if(!padded){
        block[rawDataSize] = 1;
//        for(int i = rawDataSize + 1; i < BLOCKSIZE; i++)
//            block[i] = 0;
    }
}

/*! 
 This function encrypts the block. It uses makeIV() function to create IV and AES.ENC to encrypt the block 
 */
unsigned char* DataBlock::ENC(){
    AES* cipher = new AES();
    makeIV(); //It will populate the iv parameter of the object
    unsigned char * ciphertext = NULL;
    ciphertext = cipher->ENC(block, size-(uint32_t)sizeof(version), DataBlock::key, DataBlock::iv);
    delete cipher;
    return ciphertext;
}

/*! 
 This function decrypts the block. It uses makeIV() function to create IV and AES.DEC to encrypt the block 
 */
unsigned char* DataBlock::DEC(){
    AES* cipher = new AES();
    makeIV(); //It will populate the iv parameter of the object
    unsigned char * plaintext = cipher->DEC(block, size-(uint32_t)sizeof(version), DataBlock::key, DataBlock::iv);
    delete cipher;
    return plaintext;
}

/*! 
 This function is used to generate a random key using AES.keygen.
 */
void DataBlock::generateKey(){
    AES* cipher = new AES();
    DataBlock::key = cipher->keyGen();
    printBytes("GeneratedKey", key, 16);
    delete cipher;
}

/*!
 This function can be used to set the key, if the key is already available.
 */
void DataBlock::setKey(unsigned char* key){
    DataBlock::key = key;
}

/*!
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

void DataBlock::printBytes(string tag, char* value, uint32_t size){
    cout << endl << "*********************" << tag << " (start)*********************" << endl;
    for( int i = 0; i < size; i++)
        cout << i << " --> " << (int)value[i] << '\t';
    cout << endl << "**********************" << tag << " (end)**********************" << endl;
}

/*!
 This function saves the key to the disk.
 */
void DataBlock::saveKeytoFile(){
    std::ofstream file;
    file.open(keyFilename);
    printBytes("SavedFile1", key, 16);
    file.write((char*)key, 16);
    printBytes("SavedFile2", (char*)key, 16);

    cout << endl << "Key stored!" << endl;
}

/*!
 This function loads the key from the disk. DataBlock::key = (unsigned char*) key; messes up the key (I don't know the reason), but memcpy works fine. I keep note of this to learn why the above statement doesn't work.
 */
void DataBlock::loadKeyfromFile(){
    std::ifstream file;
    file.open(keyFilename);
    char key[16];
    file.read(key,16);
//    printBytes("ReadFile1", key, 16);
//    DataBlock::key = (unsigned char*) key;
    memcpy(DataBlock::key, key, 16); ///key doesn't need to be changed to unsigned char* because values can be just read as unsigned char
//    printBytes("ReadFile2", (unsigned char*) key, 16);
//    printBytes("DataBlockReadkey", DataBlock::key, 16);
//    cout << endl << "Key read!" << endl;
}

/*!
 This function checks if the key file is present on disk. It's used to check that if the key is present on the disk, the object will not try to generate a new key.
 */
bool DataBlock::isKeyFileStored(){
    std::ifstream file;
    file.open(keyFilename);
    if(file.good())
        return true;
    else
        return false;
}

void DataBlock::setupKey(){
    if(!isKeyFileStored()){
        if(!wasKeyGenerated){
            generateKey();
            saveKeytoFile();
            wasKeyGenerated = true;
        }
    }
    else{
        if(!wasKeyGenerated){
            loadKeyfromFile();
            wasKeyGenerated = true;
        }
    }
}

/*!
 This function can be used to check if the block is occupied or not.
 */
bool DataBlock::isOccupied(){
    return fid ? false : true;
}