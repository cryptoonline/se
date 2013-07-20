//
// DiskCommunicator.cpp
// BlindStorage
//

#include "DiskCommunicator.h"

DiskCommunicator::DiskCommunicator(){
	DSource.open(D_FILE, DSize);
	DSink.open(D_FILE, DSize);

	TSource.open(T_FILE, TTOTAL_BLOCKS*TBLOCK_SIZE);
	TSink.open(T_FILE, TSize);
}

DiskCommunicator::~DiskCommunicator(){
	
	if(DSource.is_open())
		DSource.close();

	if(DSink.is_open())
		DSink.close();

	if(TSource.is_open())
		TSource.close();

	if(TSink.is_open())
		TSink.close();
}

void DiskCommunicator::dPut(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks){
	if(DSink.is_open()){
		byte* D = (byte*)DSink.data();
		for(int32_t i = 0; i < numBlocks; i++)
			memcpy(D+blockLocations[i]*BLOCK_SIZE, &blocks[i*BLOCK_SIZE], BLOCK_SIZE);
	}
	else
		cout << D_FILE << " could not be mapped." << endl;
}

void DiskCommunicator::dGet(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks){
	if(DSource.is_open()){
		byte* D = (byte*)DSource.data();
		for(int32_t i = 0; i < numBlocks; i++)
			memcpy(&blocks[i*BLOCK_SIZE], D+blockLocations[i]*BLOCK_SIZE, BLOCK_SIZE);
	}
	else 
		cout << D_FILE << " could not be mapped." << endl;
}

void DiskCommunicator::tPut(t_index_t index, byte* block){
	if(TSink.is_open()){
		byte* T = (byte*)TSink.data();
		memcpy(T+index*TBLOCK_SIZE, block, TBLOCK_SIZE);
	}
	else
		cout << T_FILE << " could not be mapped." << endl;
}

void DiskCommunicator::tGet(t_index_t index, byte* block){
	if(TSource.is_open()){
		byte* T = (byte*)TSource.data();
		memcpy(block, T+index*TBLOCK_SIZE, TBLOCK_SIZE);
	}
	else
		cout << T_FILE << " could not be mapped." << endl;
}
