//
//  Ddisk.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "Ddisk.h"

Ddisk::Ddisk(Communicator communicator){
    this->communicator = communicator;
    numBlocks = TOTAL_BLOCKS;
    D = new DataBlock*[numBlocks];
    for(int i = 0; i < numBlocks; i++)
        D[i] = new DataBlock();
}

Ddisk::Ddisk(uint32_t numBlocks){
    this->numBlocks = numBlocks;
    D = new DataBlock*[numBlocks];
    for(int i = 0; i < numBlocks; i++)
        D[i] = new DataBlock();
}

//void Ddisk::addBlock(uint64_t blockIndex, fileID &fid, unsigned char* rawData, uint32_t size){
//    D[blockIndex] = new DataBlock(blockIndex, fid, rawData, size);
//}

void Ddisk::addFile(string filename){
	size_t size = readFileSize(filename);
	PRSubset prSubset(ceil((double)size/MAX_BLOCK_DATA_SIZE) * BLOW_UP);
	addFile(filename, prSubset);
}

void Ddisk::addFile(string filename, PRSubset& prSubset){
	fileID fid(filename);
	size_t size = readFileSize(filename);

	unsigned char* fileBytes = reinterpret_cast<unsigned char*>(readFileBytes(filename, size));

	addBlocks(fileBytes, size, fid, prSubset);
} 

void Ddisk::addFile(unsigned char* fileBytes, uint32_t fileSize, fileID &fid, PRSubset &prSubset){
    addBlocks(fileBytes, fileSize, fid, prSubset);
}

void Ddisk::addBlocks(unsigned char* fileBytes, size_t filesize, fileID &fid, PRSubset &prSubset){
    vector<uint32_t> emptyBlocks = getEmptyBlocks(prSubset);
//	cout << "Required block: " << ceil((double)filesize/MAX_BLOCK_DATA_SIZE) << "\tAvailable blocks: " << emptyBlocks.size() << endl;
	if(emptyBlocks.size()*MAX_BLOCK_DATA_SIZE < filesize){
		cerr << "Not enough empty blocks" << endl;
        exit(1);
    }
    makeBlocks(fileBytes, emptyBlocks.data(), fid, filesize);
    
//    makeBlocks(fileBytes, prSubset.get(), fid, filesize);
}

void Ddisk::makeBlocks(unsigned char* fileBytes, uint32_t* prSubset, fileID &fid, size_t filesize){
    int counter = 0;
    int32_t numBlocks = (int32_t)ceil((double)filesize/(double)MAX_BLOCK_DATA_SIZE);
    unsigned char* blocks = new unsigned char[numBlocks*BLOCK_SIZE]();
    
    for(; counter < numBlocks - 2; counter++){
        memcpy(&blocks[counter * BLOCK_SIZE], &fileBytes[counter*MAX_BLOCK_DATA_SIZE] , MAX_BLOCK_DATA_SIZE);
        //cout << "Processing Block " << counter << endl;
//        print("Blocks", &fileBytes[counter*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
        D[prSubset[counter]] = new DataBlock(prSubset[counter], fid, &blocks[counter * BLOCK_SIZE], MAX_BLOCK_DATA_SIZE);
//        print("Decrypted Blocks", D[prSubset[counter]]->getDecrypted(), BLOCK_SIZE);
    }
//    print(__PRETTY_FUNCTION__, prSubset, numBlocks);

    uint32_t sizeOfLastBlock = (uint32_t)filesize - ((uint32_t)filesize/MAX_BLOCK_DATA_SIZE) * MAX_BLOCK_DATA_SIZE;
    memcpy(&blocks[counter * BLOCK_SIZE], &fileBytes[counter*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
    D[prSubset[counter]] = new DataBlock(prSubset[counter], fid, &blocks[counter * BLOCK_SIZE], sizeOfLastBlock);
}

char* Ddisk::readFileBytes(string filename, size_t size){
	ifstream file(filename.c_str());
    char* bytes = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(bytes, size);
    file.close();
    return bytes;
}

size_t Ddisk::readFileSize(string filename){	
    ifstream file(filename.c_str());
	file.seekg(0, std::ios::end);
    size_t filesize = file.tellg();
	return filesize;
}

vector<uint32_t> Ddisk::getEmptyBlocks(PRSubset &prSubset){
    uint32_t prSubsetSize = prSubset.getSize();
    uint32_t* subset = prSubset.get();
    std::vector<uint32_t> emptyBlocks;
    for(int i = 0; i < prSubsetSize; i++){
        if(!D[subset[i]]->isOccupied())
            emptyBlocks.push_back(subset[i]);
    }
    
//    cout << "----------------------------------------------------------------------------------------------------" << endl;
//    for(int i = 0; i < emptyBlocks.size(); i++)
//        cout <<  emptyBlocks[i] << "\t";
//    cout << endl << "----------------------------------------------------------------------------------------------------" << endl;
//    
    return emptyBlocks;
}

void Ddisk::finalize(){
    for(int i = 0; i < numBlocks; i++)
        if(!D[i]->isOccupied()){
            D[i]->encryptIfEmpty();
        }
}

void Ddisk::upload(){
    
}

void Ddisk::writeToDisk(){
	ofstream file("data/D");
	for(int i = 0; i < numBlocks; i++)
		file.write(reinterpret_cast<char*>(D[i]->getEncrypted()), BLOCK_SIZE);
}

void Ddisk::print(string tag, uint32_t* subset, uint32_t size){
    for(int i = 0; i < size; i++){
        print(tag, D[subset[i]]->getDecrypted(), BLOCK_SIZE);
    }
}

DataBlock** Ddisk::get(){
    return D;
}

void Ddisk::print(string tag, unsigned char* value, uint32_t size){
    cout << endl << "*********************" << tag << " (start)*********************" << endl;
    for( int i = 0; i < MAX_BLOCK_DATA_SIZE; i++)
        cout << value[i];
    cout << endl << "Trailer: ";
    for(int j = MAX_BLOCK_DATA_SIZE; j < BLOCK_SIZE; j++)
        cout << (int)value[j] << "\t";
    cout << endl << "**********************" << tag << " (end)**********************" << endl;
}

void Ddisk::print(string tag, char* value, uint32_t size){
    cout << endl << "*********************" << tag << " (start)*********************" << endl;
    for( int i = 0; i < size; i++)
        cout << value[i];
    cout << endl << "**********************" << tag << " (end)**********************" << endl;
}



