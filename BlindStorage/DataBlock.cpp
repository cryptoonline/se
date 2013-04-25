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

bool DataBlock::wasKeyGenerated = false;
unsigned char* DataBlock::key = new unsigned char[16];
const char* DataBlock::keyFilename = "secret.txt";

///*!
// DataBlock constructor.
// */
//DataBlock::DataBlock(){
//    version = 0; /*!< Initialized to 0*/
//    block = new unsigned char[size]();
//    iv = new unsigned char[16];
//    setupKey();
//}

/*!
 Constructor for the online phase, will return decrypted block
 */

void DataBlock::initialize(){
    iv = new unsigned char[16]();
    setupKey();
    fid = new fileID();
    
}

DataBlock::DataBlock(){
    initialize();
    this->block = new unsigned char[BLOCK_SIZE]();
    version = 0;
}

DataBlock::DataBlock(uint32_t blockIndex, unsigned char* block){
    initialize();
    if(this->block == NULL)
        delete[] this->block;
    this->block = block;
    parseBlock();
    higherFid = fid->getPRPofHigherID(); /// This should be run after parsing the block
}

DataBlock::DataBlock(uint32_t blockIndex, fileID &fid, unsigned char* rawData, uint32_t size = MAX_BLOCK_DATA_SIZE){
    initialize();
    version = 0;
    higherFid = fid.getPRPofHigherID();
    block = new unsigned char[size]();
    add(blockIndex, fid, rawData, size);
    makeBlock();
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
    return block;
}

unsigned char* DataBlock::getDecrypted(){
    return DEC();
//    memcpy(block, decryptedblock, BLOCK_SIZE - sizeof(version) );
//    return block;
}
    
/*!
 This functions accepts data size as input to handle the case when the data is less than the BLOCKSIZE.
 */
void DataBlock::update(uint32_t blockIndex, fileID &fid, unsigned char* rawData, uint32_t size){
    ++version; ///version should be incremented before adding the block
    add(blockIndex, fid, rawData, size);
    makeBlock();
}

void DataBlock::add(uint32_t blockIndex, fileID &fid, unsigned char* rawData, uint32_t size){
    this->rawData = rawData;
    this->blockIndex = blockIndex;
    this->fid = &fid;
    
    if(size < MAX_BLOCK_DATA_SIZE){
        this->rawDataSize = size;
        padded = 0;
    }
    else if (size == MAX_BLOCK_DATA_SIZE){
        this->rawDataSize = MAX_BLOCK_DATA_SIZE;
        padded = 1;
    }
    else{
        cout << "Size of the block is large than the acceptable size.";
    }
}

/*! Create complete block of the following format ENC(rawData||paddedbit||fid)||version */
void DataBlock::makeBlock(){
    uint32_t pointer = 0;
    
    block = rawData;

    if(!padded)
        addPadding();
    pointer += MAX_BLOCK_DATA_SIZE;
    
    block[pointer] = padded;
    pointer += 1;
        
    memcpy(&block[pointer], fid->get(), 32);
    pointer += 32;
    
    const byte* encryptedBlock = ENC();
    memcpy(block, encryptedBlock,BLOCK_SIZE-(uint32_t)sizeof(version));
    delete[] encryptedBlock;
    
    memcpy(&block[pointer], static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(uint32_t));
}

void DataBlock::parseBlock(){
    version = *(uint32_t *)(&block[BLOCK_SIZE-sizeof(uint32_t)]);
    fileID fid(&block[MAX_BLOCK_DATA_SIZE+1]);
    this->fid = &fid;
    padded = *(unsigned char *)(&block[MAX_BLOCK_DATA_SIZE+1]);
    if(!padded)
        removePadding();
}

/*! 
 Use the pad the block when its less than required number of bytes 
 */
void DataBlock::addPadding(){
        block[rawDataSize] = 1;
}

void DataBlock::removePadding(){
    for(int i = MAX_BLOCK_DATA_SIZE; i > 0; i--)
        if(block[i] == 1){
            rawDataSize = i-1;
        }
}

bool DataBlock::checkFileID(fileID &fid){
    unsigned char* blockFid = this->fid->get();
    unsigned char* fidToCheck = fid.get();
    for(int i = 0; i < 32; i++)
        if(fidToCheck[i] != blockFid[i])
            return false;
    return true;
}

/*!
 This function encrypts the block. It uses makeIV() function to create IV and AES.ENC to encrypt the block 
 */
unsigned char* DataBlock::ENC(){
    AES* cipher = new AES();
    makeIV(); //It will populate the iv parameter of the object
    unsigned char * ciphertext = cipher->ENC(block, BLOCK_SIZE-(uint32_t)sizeof(version), DataBlock::key, DataBlock::iv);
    delete cipher;
    return ciphertext;
}

/*! 
 This function decrypts the block. It uses makeIV() function to create IV and AES.DEC to encrypt the block 
 */
unsigned char* DataBlock::DEC(){
    AES* cipher = new AES();
    makeIV(); //It will populate the iv parameter of the object
    unsigned char * plaintext = cipher->DEC(block, BLOCK_SIZE-(uint32_t)sizeof(version), DataBlock::key, DataBlock::iv);
    delete cipher;
    return plaintext;
}

/*! 
 This function is used to generate a random key using AES.keygen.
 */
void DataBlock::generateKey(){
    AES* cipher = new AES();
    DataBlock::key = cipher->keyGen();
    delete cipher;
}

///*!
// This function can be used to set the key, if the key is already available.
// */
//void DataBlock::setKey(unsigned char* key){
//    DataBlock::key = key;
//}

/*!
 This function is used to create IV using blockIndex and version. The format of the IV is as:
 MSB --- LSBofblockIndex - MSBofblockIndex, LSBofversion-MSBofversion, 
 */
void DataBlock::makeIV(){
    memcpy(iv, static_cast<unsigned char*>(static_cast<void*>(&blockIndex)), sizeof(blockIndex));
    memcpy(iv+8, static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(uint32_t));
//    unsigned char zeros[8] = {0};
//    memcpy(iv+8, zeros, 8);
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
        cout << (int)value[i] << "\t";
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
const bool DataBlock::isKeyFileStored(){
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
const bool DataBlock::isOccupied(){
    if(higherFid)
    cout << "Is Occupied";
    return higherFid ? true : false;
}

void DataBlock::encryptIfEmpty(){
    ENC();
}