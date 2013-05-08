//
// Cri.cpp
// BlindStorage
//

#include "Cri.h"

Cri::Cri(){
}

void Cri::addTBlock(TBlock& tblock){
	if(tblock.getPrSubsetSize() == 0)
		prSubset = new PRSubset(4);
	else
		prSubset = new PRSubset(tblock.getPrSubsetSize(), tblock.getPrSubsetSeed());
	
	cout << "PrSubset size is " << prSubset->getSize() << " and seed is " << prSubset->getSeed() << endl;
	getchar();
	blocksLocations = prSubset->get();
	numBlocks = prSubset->getSize();
	blocks = new DataBlock*[numBlocks];
	extractedBlocks = new DataBlock*[numBlocks/BLOW_UP];
	plaintextBlocks = new unsigned char*[numBlocks];
	extractedPlaintextBlocks = new unsigned char*[numBlocks/BLOW_UP];
	
	numEntries = numBlocks * MAX_BLOCK_DATA_SIZE / CRI_ENTRY_SIZE;
	entries = new unsigned char*[numEntries];
}

void Cri::addFile(unsigned char** file, fileID& fid){
	this->fid = &fid;
//	this->fid->makeCRIID();
	printhex(this->fid->get(), 32, "FID");
	getchar();
	ciphertextBlocks = file;
	decrypt();
}

void Cri::decrypt(){
	unsigned char entriesData[numBlocks*MAX_BLOCK_DATA_SIZE];
	int j = 0;
	for(int i = 0; i < numBlocks; i++){
		blocks[i] = new DataBlock(blocksLocations[i], ciphertextBlocks[i]);
		plaintextBlocks[i] = blocks[i]->getDecrypted();
		memcpy(&entriesData[i*MAX_BLOCK_DATA_SIZE], plaintextBlocks[i], MAX_BLOCK_DATA_SIZE);
//		if(blocks[i]->checkCRIFileID(*fid)){
//			extractedBlocks[j] = blocks[i];
//			extractedPlaintextBlocks[j] = plaintextBlocks[i];
//			j++;
//		}
	}
	for(int i = 0; i < numEntries; i++){
		entries[i] = new unsigned char[CRI_ENTRY_SIZE]();
		memcpy(entries[i], &entriesData[i*CRI_ENTRY_SIZE], CRI_ENTRY_SIZE);
	}	
}

int32_t Cri::searchFID(unsigned char* arrayToSearchFor){
	
	int matches;

//	printhex(extractedPlaintextBlocks, numBlocks/BLOW_UP, BLOCK_SIZE,"EXTRACTED");
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

int32_t Cri::getEmptyCriEntry(){
	for(int i = 0; i < numBlocks; i++){
		if( *(uint64_t*)entries[i] == 0)
			return i;
	}
}

void Cri::makeCriBlock(unsigned char* criEntry){
	int32_t index = getEmptyCriEntry();
	memcpy(entries[index], criEntry, CRI_ENTRY_SIZE);
	for(int i = 0; i < numBlocks; i++){
		blocks[i]->update(*fid, entries[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
	}
}

void Cri::writeCriEntry(unsigned char* D, unsigned char* criEntry){
	makeCriBlock(criEntry);
	for(int i = 0; i < numBlocks; i++)
		memcpy(&D[blocksLocations[i]], blocks[i]->getEncrypted(), BLOCK_SIZE);
}

PRSubset* Cri::getCRIPrSubset(){
	return prSubset;
}
