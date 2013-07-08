//
// DiskCommunicator.cpp
// BlindStorage
//

#include "DiskCommunicator.h"

byte* DiskCommunicator::T = NULL;
byte* DiskCommunicator::D = NULL;

DiskCommunicator::DiskCommunicator(){
	TSize = readFileSize(T_FILE);
	DSize = readFileSize(D_FILE);
	
	T = new byte[TSize];
	D = new byte[DSize];

	memset(T, 0, TSize);
	memset(D, 0, DSize);
	
	cout << "Reading T from disk..." << endl;
	loadFile(T_FILE, T, TSize);
	cout << "T successfully read into memory." << endl;

	cout << "Reading D from disk..." << endl;
	loadFile(D_FILE, D, DSize);
	cout << "D successfully read into memory." << endl;

//	printhex(T, TSize, "T");
//	for(int i = 0; i < TOTAL_BLOCKS; i++){
//		cout << "Block " << i << endl;
//		printhex(&D[i*BLOCK_SIZE], BLOCK_SIZE, "DiskCommunicator");
//	}
}

DiskCommunicator::~DiskCommunicator(){
	delete[] T;
	delete[] D;
}

void DiskCommunicator::dPut(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks){
	for(int i = 0; i < numBlocks; i++)
		memcpy(&D[blockLocations[i]*BLOCK_SIZE], &blocks[i*BLOCK_SIZE], BLOCK_SIZE);
}

void DiskCommunicator::dGet(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks){
	for(int i = 0; i < numBlocks; i++){
		memcpy(&blocks[i*BLOCK_SIZE], &D[blockLocations[i]*BLOCK_SIZE], BLOCK_SIZE);
	}
}

void DiskCommunicator::tPut(t_index_t index, byte* block){
	memcpy(&T[index*TBLOCK_SIZE], block, TBLOCK_SIZE);
}

void DiskCommunicator::tGet(t_index_t index, byte* block){
	memcpy(block, &T[index*TBLOCK_SIZE], TBLOCK_SIZE);
}

void DiskCommunicator::loadFile(string filename, byte* input, size_t size){
	ifstream file(filename.c_str());
	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(input), size);
	file.close();
}

size_t DiskCommunicator::readFileSize(string path){
	struct stat st;
	stat(path.c_str(), &st);
	return st.st_size;
}
