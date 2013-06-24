//
//  Ddisk.cpp
//  BlindStorage
//

#include "Ddisk.h"

Ddisk::Ddisk(b_index_t numBlocks){
	this->numBlocks = numBlocks;

	D = new DataBlock*[TOTAL_BLOCKS];
	for(b_index_t i = 0; i < TOTAL_BLOCKS; i++)
		D[i] = new DataBlock(i);

	Key key("DdiskKey", AES_KEY_SIZE);
	byte keyBytes[AES_KEY_SIZE];
	key.get(keyBytes);
	
	D[0]->setKey(keyBytes);

}

Ddisk::~Ddisk(){
	delete[] D;
}

void Ddisk::makeBlocks(byte bytes[], size_t size, fileID fid, vector<b_index_t>& emptyBlocks, bool isCRI){
// Bytes in this function should be interleaved for the amount of bytes needed by DataBlock trailer
	b_index_t requiredNumBlocks = (b_index_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);

	int32_t counter = 0;
	for(; counter < requiredNumBlocks - 1; counter++){
		D[emptyBlocks[counter]]->make(fid, &bytes[counter*BLOCK_SIZE], MAX_BLOCK_DATA_SIZE, isCRI);
	}

	dataSize_t sizeOfLastBlock = (dataSize_t)size - (dataSize_t)(size/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE;
	D[emptyBlocks[counter]]->make(fid, &bytes[counter*BLOCK_SIZE], sizeOfLastBlock, isCRI); 
}

void Ddisk::getEmptyBlocks(PRSubset prSubset, vector<b_index_t>& emptyBlocks){
	prSubsetSize_t size = prSubset.getSize();
	b_index_t subset[size];
	prSubset.get(subset, size);

	for(int i = 0; i < size; i++){
		if(!(D[subset[i]]->isOccupied()))
			emptyBlocks.push_back(subset[i]);
	}
	cout << "Size is " << emptyBlocks.size() << endl;
}

void Ddisk::addFile(byte bytes[], size_t size, fileID fid, PRSubset prSubset, bool isCRI){
	vector<b_index_t> emptyBlocks;
	getEmptyBlocks(prSubset, emptyBlocks);
	if(emptyBlocks.size()*MAX_BLOCK_DATA_SIZE < size){
		cerr << "Not enough empty blocks." << endl;
		exit(1);
	}
	makeBlocks(bytes, size, fid, emptyBlocks, isCRI); 
}

void Ddisk::getBlock(b_index_t index, DataBlock& block){
	block = *D[index];
}

void Ddisk::encryptEmptyBlocks(){
	byte zeroBlock[BLOCK_SIZE] = {0};
	for(int i = 0; i < numBlocks; i++)
		if(!D[i]->isOccupied())
			D[i]->encryptIfEmpty(zeroBlock);
}
