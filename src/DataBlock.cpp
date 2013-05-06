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
    //iv = new unsigned char[16];
    setupKey();
//    fid = new fileID();    
}

DataBlock::DataBlock(uint32_t blockIndex){
	//iv = new unsigned char[16];
//	this->block = new unsigned char[BLOCK_SIZE]();
	ciphertextBlock = new unsigned char[BLOCK_SIZE]();
	plaintextBlock = new unsigned char[BLOCK_SIZE]();
    version = 0;
	setupKey();
	this->blockIndex = blockIndex;
	higherFid = 0;
}

DataBlock::DataBlock(uint32_t blockIndex, unsigned char* block){
	this->plaintextBlock = new unsigned char[BLOCK_SIZE]();
	this->blockIndex = blockIndex;
	setupKey();
   // this->block = block;
	this->ciphertextBlock = block;
    parseBlock();
	printhex(fid->get(), 32, "FID in DATABLOCK");

    higherFid = fid->getPRPofHigherID(); /// This should be run after parsing the block
	cout << "HIGHER FID " << higherFid << " for index " << blockIndex << endl;
//	cout << "BLOCK INDEX " << this->blockIndex << endl;
	//printhex(ciphertextBlock, BLOCK_SIZE, "GET BLOCKS ENCRYPTED");
}

DataBlock::DataBlock(uint32_t blockIndex, fileID &fid, unsigned char* rawData, uint32_t size = MAX_BLOCK_DATA_SIZE){
	ciphertextBlock = new unsigned char[BLOCK_SIZE]();
	//printhex(ciphertextBlock, BLOCK_SIZE, "EMPTY CIPHERTEXT ARRAY");
	plaintextBlock = new unsigned char[BLOCK_SIZE]();
	//printchars(rawData, size, "ACTUAL DATA");
	this->blockIndex = blockIndex;
	setupKey();
    version = 0;
    higherFid = fid.getPRPofHigherID();
   // block = new unsigned char[size]();
    add(blockIndex, fid, rawData, size);
    makeBlock();	
	cout << "BLOCK INDEX " << this->blockIndex << endl;
	//printhex(ciphertextBlock, BLOCK_SIZE, "BUILD BLOCKS ENCRYPTED");
}

/*!
 DataBlock destructor.
 */
DataBlock::~DataBlock(){
    delete[] rawData;
   // delete[] key;
    //delete[] iv;
}
    
/*!
 This function can be used to retrieve the decrypted
 */
unsigned char* DataBlock::getEncrypted(){
   // return block;
	return ciphertextBlock;
}

unsigned char* DataBlock::getDecrypted(){
    return plaintextBlock;
//    memcpy(block, decryptedblock, BLOCK_SIZE - sizeof(version) );
//    return block;
}
    
/*!
 This functions accepts data size as input to handle the case when the data is less than the BLOCKSIZE.
 */
void DataBlock::update(fileID &fid, unsigned char* rawData, uint32_t size){
    ++version; ///version should be incremented before adding the block
    if(rawData)
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
   
	memcpy(plaintextBlock, rawData, MAX_BLOCK_DATA_SIZE); 
    //plaintextBlock = rawData;

    if(!padded)
        addPadding();
    pointer += MAX_BLOCK_DATA_SIZE;
    
    plaintextBlock[pointer] = padded;
    pointer += 1;
       
	//printhex(fid->get(), 32, "FID in MAKE BLOCKS"); 
    memcpy(&plaintextBlock[pointer], fid->get(), 32);
	//printhex(&block[pointer], 32, "FID in MAKE after its copied to BLOCK");
    pointer += 32;
   
	//printhex(plaintextBlock, BLOCK_SIZE, "Whole Decrypted block in MAKE BLOCK"); 
    byte* encryptedBlock = ENC();
	//printhex(encryptedBlock, BLOCK_SIZE, "ENCRYPTED BLOCK");
	//printhex(ciphertextBlock, BLOCK_SIZE, "EMPTY CIPHERTEXT ARRAY");
    memcpy(ciphertextBlock, encryptedBlock, BLOCK_SIZE-(uint32_t)sizeof(version));
    delete[] encryptedBlock;;
    
    //memcpy(&block[pointer], static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(uint32_t)); //Initially the array is intialized to zeros so we don't need to do this.
}

void DataBlock::parseBlock(){
//	printhex(block, BLOCK_SIZE, "block in parse block");
    version = *(uint32_t *)(&ciphertextBlock[BLOCK_SIZE-sizeof(uint32_t)]);

//	this->plaintextBlock = new unsigned char[BLOCK_SIZE]();
	memcpy(&plaintextBlock[BLOCK_SIZE - sizeof(version)], &ciphertextBlock[BLOCK_SIZE-sizeof(version)], 4);
	unsigned char* decryptedBlock = DEC();
	memcpy(plaintextBlock, decryptedBlock, BLOCK_SIZE);
	//printhex(plaintextBlock, BLOCK_SIZE, "DECRYPTED BLOCK");
	//printchars(plaintextBlock, BLOCK_SIZE, "DECRYPTED BLOCK CONTENTS");
//	delete[] plaintextBlock;
    fileID* fid = new fileID(&plaintextBlock[MAX_BLOCK_DATA_SIZE+1]);
    this->fid = fid;
	//printhex(fid->get(), 32, "FID in PARSE");
	//printhex(this->fid->get(), 32, "FID in PARSE OBJECT");
    padded = *(unsigned char *)(&plaintextBlock[MAX_BLOCK_DATA_SIZE]);
    if(!padded)
        removePadding();
}

//void DataBlock::deleteBlock(){
//    version++;
//    uint32_t zeros[32] = {0};
//    uint32_t pointer = MAX_BLOCK_DATA_SIZE + 1;
//    memcpy(&block[pointer], zeros, 32);
//    const byte* ciphertextBlock = ENC();
//    memcpy(block, ciphertextBlock, BLOCK_SIZE-(uint32_t)sizeof(version));
//    delete[] ciphertextBlock;
//    memcpy(&block[pointer], static_cast<unsigned char*>(static_cast<void*>(&version)), sizeof(uint32_t));
//}

/*! 
 Use the pad the block when its less than required number of bytes 
 */
void DataBlock::addPadding(){
        plaintextBlock[rawDataSize] = 1;
}

void DataBlock::removePadding(){
    for(int i = MAX_BLOCK_DATA_SIZE; i > 0; i--)
        if(plaintextBlock[i] == 1){
            rawDataSize = i-1;
        }
}

bool DataBlock::checkFileID(fileID &fid){
    unsigned char* blockFid = this->fid->get();
	//printhex(blockFid, 32, "Datablock FID");
    unsigned char* fidToCheck = fid.get();
	//printhex(fid.get(), 32, "Required File FID");
    for(int i = 0; i < 32; i++)
        if(fidToCheck[i] != blockFid[i])
            return false;
    return true;
}

/*!
 This function encrypts the block. It uses makeIV() function to create IV and AES.ENC to encrypt the block 
 */
unsigned char* DataBlock::ENC(){
    AES cipher;
    makeIV(); //It will populate the iv parameter of the object
    unsigned char * ciphertext = cipher.ENC(plaintextBlock, BLOCK_SIZE-(uint32_t)sizeof(version), DataBlock::key, DataBlock::iv);
    return ciphertext;
}

/*! 
 This function decrypts the block. It uses makeIV() function to create IV and AES.DEC to encrypt the block 
 */
unsigned char* DataBlock::DEC(){
    AES cipher;
    makeIV(); //It will populate the iv parameter of the object
//	printhex(iv, 16, "Decryption IV");
    unsigned char * plaintext = cipher.DEC(ciphertextBlock, BLOCK_SIZE-(uint32_t)sizeof(version), DataBlock::key, DataBlock::iv);
    return plaintext;
}

/*! 
 This function is used to generate a random key using AES.keygen.
 */
void DataBlock::generateKey(){
    AES cipher;
    DataBlock::key = cipher.keyGen();
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
	iv = new unsigned char[16]();
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
//    file.open(keyFilename);
	file.open("secret.txt");
    printBytes("SavedFile1", key, 16);
    file.write((char*)key, 16);
    printBytes("SavedFile2", (char*)key, 16);

    cout << endl << "Key stored!" << endl;
	file.close();
}

/*!
 This function loads the key from the disk. DataBlock::key = (unsigned char*) key; messes up the key (I don't know the reason), but memcpy works fine. I keep note of this to learn why the above statement doesn't work.
 */
void DataBlock::loadKeyfromFile(){
    std::ifstream file;
//    file.open(keyFilename);
	file.open("secret.txt");
    char key[16];
    file.read(key,16);
	file.close();
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
//    file.open(keyFilename);
	file.open("secret.txt");
    if(file.good()){
        file.close();
		return true;
	}
    else{
		file.close();
        return false;
	}
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
	cout << "Index in " << __PRETTY_FUNCTION__ << " is " << blockIndex << endl;
	cout << __PRETTY_FUNCTION__ << " higher FID is " << higherFid << endl;
    return higherFid ? true : false;
}

void DataBlock::encryptIfEmpty(){
    unsigned char* ciphertext = ENC();
	memcpy(ciphertextBlock, ciphertext, BLOCK_SIZE-(uint32_t)sizeof(version));
	delete ciphertext;
}
