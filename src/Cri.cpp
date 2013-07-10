//
// Cri.cpp
// BlindStorage
//

#include "Cri.h"

CRI::CRI(){
}

CRI::CRI(higherfid_t higherfid){
	empty = true;
	this->higherfid = higherfid;
//	blocks.resize(2);
}

CRI::~CRI(){
}

void CRI::addFile(prSubsetSize_t size, prSubsetSeed_t seed, byte lowerFid[]){
	CRIBlock block;
	block.make(size, seed, lowerFid);
	blocks.push_back(block);
	empty = false;
}

void CRI::updateFile(prSubsetSize_t size, prSubsetSeed_t seed, uint32_t blockIndex){
	blocks[blockIndex].update(seed, size);
}

void CRI::makeBytes(byte* blocksBytes){
	int pointer = 0;
	for(vector<CRIBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it){
		byte block[CRI_BLOCK_SIZE];
		it->get(block);
		memcpy(&blocksBytes[pointer], block, CRI_BLOCK_SIZE);
		pointer += CRI_BLOCK_SIZE;
	}
}

void CRI::makeBytes(vector<byte>& blocksBytes, string function){
	int counter = 0;
	int numPaddingBytes = MAX_BLOCK_DATA_SIZE - CRI_PER_BLOCK * CRI_BLOCK_SIZE;// + TRAILER_SIZE;
	byte zeros[numPaddingBytes];
	memset(zeros, 0, numPaddingBytes);
	cout << "Function is " << function << " Time is " << clock() << endl;
	for(vector<CRIBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it){
		vector<byte> block;
		it->get(block);
		blocksBytes.insert(blocksBytes.end(), block.begin(), block.begin()+CRI_BLOCK_SIZE);
		printhex(block, __PRETTY_FUNCTION__);
		counter++;
		if(counter % CRI_PER_BLOCK == 0 && counter != 0)
			blocksBytes.insert(blocksBytes.end(), &zeros[0], &zeros[numPaddingBytes]);
	}
//	if(blocksBytes.size() % BLOCK_SIZE != 0){
//		int numZeros = BLOCK_SIZE*(int)ceil((double)blocksBytes.size()/(double)BLOCK_SIZE)-blocksBytes.size();
//		byte zeros[numZeros];
//		memset(zeros, 0, numZeros);
//		blocksBytes.insert(blocksBytes.end(), &zeros[0], &zeros[numZeros]);
//	}
}

// void CRI::parseBytes(vector<byte>& blocksBytes){
// 	for(int i = 0; i < blocksBytes.size(); i+=CRI_BLOCK_SIZE){
// 		byte block[CRI_BLOCK_SIZE];
// 		std::copy(blocksBytes.begin()+i*CRI_BLOCK_SIZE, blocksBytes.begin()+i*CRI_BLOCK_SIZE+CRI_BLOCK_SIZE, block);
// 		CRIBlock criBlock;
// 		criBlock.parse(block);
// 		blocks.push_back(criBlock);
// 	}
// }

// void CRI::parseBytes(byte blocksBytes[], uint32_t size){
// 	for(int i = 0; i < size; i += CRI_BLOCK_SIZE){
//		byte block[CRI_BLOCK_SIZE];
//		CRIBlock criBlock;
//		criBlock.parse(block);
//		if(criBlock.getSize() == 0){
//			if(i==0)
//				empty = true;
//			break;
//		}
//		blocks.push_back(criBlock);
//		empty = false;
//	}
//}

void CRI::parseBytes(byte blocksBytes[], uint32_t size){
	int criCounter = 0;
	for(int i = 0; i < size; i+=CRI_BLOCK_SIZE){
		if(criCounter%CRI_PER_BLOCK == 0 && i!=0)
			i += MAX_BLOCK_DATA_SIZE - CRI_BLOCK_SIZE * CRI_PER_BLOCK;// + TRAILER_SIZE;
 		criCounter++;
 		byte block[CRI_BLOCK_SIZE];
 		memcpy(block, &blocksBytes[i], CRI_BLOCK_SIZE);
 		CRIBlock criBlock;
 		criBlock.parse(block);
		printhex(block, CRI_BLOCK_SIZE, __PRETTY_FUNCTION__);
 		if(criBlock.getSize() == 0){
 			if(i==0)
 				empty = true;
 			break;
 		}
 	//	blocks.resize(10);
 		blocks.push_back(criBlock);
 		empty = false;
 	}
}

criSize_t CRI::size(){
	return blocks.size() * CRI_BLOCK_SIZE;
}

uint32_t CRI::search(fileID fid, CRIBlock& block){
	cout << "CRI searching!" << endl;
	byte lowerFid[LOWERFID_SIZE];
	fid.getLowerID(lowerFid);
	return search(lowerFid, block);
}

uint32_t CRI::search(byte lowerFid[], CRIBlock& block){
	for(int i = 0; i < blocks.size(); i++){
		if(blocks[i].match(lowerFid)){
			block = blocks[i];
			cout << "Match Found" << endl;
			return i;
		}
	}
	return -1;
}

bool CRI::isEmpty(){
	return empty;
}
