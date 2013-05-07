//
// Cri.cpp
// BlindStorage
//

#include "Cri.h"

Cri::Cri(){
}

void Cri::addTBlock(TBlock& tblock){
	prSubset = new PRSubset(tblock.getPrSubsetSize(), tblock.getPrSubsetSeed());
	cout << "PrSubset size is " << prSubset->getSize() << " and seed is " << prSubset->getSeed() << endl;
	blocksLocations = prSubset->get();
	numBlocks = prSubset->getSize();
	blocks = new DataBlock*[numBlocks];
	plaintextBlocks = new unsigned char*[numBlocks];
	
	numEntries = numBlocks * MAX_BLOCK_DATA_SIZE / CRI_ENTRY_SIZE;
	entries = new unsigned char*[numEntries];
}

void Cri::addFile(unsigned char** file){
	ciphertextBlocks = file;
	decrypt();
}

void Cri::decrypt(){
	unsigned char entriesData[numBlocks*MAX_BLOCK_DATA_SIZE];
	for(int i = 0; i < numBlocks; i++){
		blocks[i] = new DataBlock(blocksLocations[i], ciphertextBlocks[i]);
		plaintextBlocks[i] = blocks[i]->getDecrypted();
		memcpy(&entriesData[i*MAX_BLOCK_DATA_SIZE], plaintextBlocks[i], MAX_BLOCK_DATA_SIZE);
	}
	for(int i = 0; i < numEntries; i++){
		entries[i] = new unsigned char[CRI_ENTRY_SIZE]();
		memcpy(entries[i], &entriesData[i*CRI_ENTRY_SIZE], CRI_ENTRY_SIZE);
	}	
}

int32_t Cri::searchFID(unsigned char* arrayToSearchFor){
	
	int matches;

	printhex(plaintextBlocks, numBlocks, BLOCK_SIZE, "BLOCKS");
//	printhex(entriesData, numEntries*MAX_BLOCK_DATA_SIZE, "ENTRIES DATA");	
	printhex(arrayToSearchFor, 32, "FID");
	printhex(entries, numEntries, 32, "ENTRIES");
	for(int i = 0; i < numEntries; i++){
		matches = 0;
		for(int j = 0; j < CRI_ENTRY_SIZE; j++){
			cout << (int)arrayToSearchFor[j] << "\t" << (int)entries[i][j+8] << endl;
			if(arrayToSearchFor[j] == entries[i][j+8]){
				matches++;
				if(matches == 32)
					return i;
				}
			else
				continue;
		}		
	}
	return -1;
}

uint32_t* Cri::getBlocksLocations(){
	return this->blocksLocations;
//	blocksLocations = this->blocksLocations;
//	printdec(blocksLocations, numBlocks, "CRI");
}

uint32_t Cri::getNumBlocks(){
	return numBlocks;
}

unsigned char* Cri::getEntry(int32_t index){
	cout << "Seed " << *(uint32_t*)entries[index] << " and Size " << *(uint32_t*)(&entries[index]+4) << endl;
	return entries[index];
}
