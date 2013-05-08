//
// DataStructures.cpp
// BlindStorage
//

#include "DataStructures.h"

char* DataStructures::T = NULL;
char* DataStructures::D = NULL;

DataStructures::DataStructures(){
	string TF = T_FILE;
	string DF = D_FILE;
	
	TSize = readFileSize(TF);
	DSize = readFileSize(DF);
	
	T = new char[TSize];
	D = new char[DSize];
	
	cout << "Reading T from disk..." << endl;
	loadFile(TF, T, TSize);
	cout << "T successfully read into memory." << endl;

	cout << "Reading D from disk..." << endl;
	loadFile(DF, D, DSize);
	cout << "D successfully read into memory." << endl;
}

void DataStructures::dPut(uint32_t* blockLocations, uint32_t numBlocks, unsigned char** blocks){
	for(int i = 0; i < numBlocks; i++)
		memcpy(&D[blockLocations[i]*BLOCK_SIZE], blocks[i], BLOCK_SIZE);
}

void DataStructures::dGet(uint32_t* blockLocations, uint32_t numBlocks, unsigned char**  blocks){
	for(int i = 0; i < numBlocks; i++)
		memcpy(blocks[i], &D[blockLocations[i]*BLOCK_SIZE], BLOCK_SIZE);
}

void DataStructures::tPut(uint32_t index, unsigned char* block){
	memcpy(&T[index*T_BLOCK_SIZE], block, T_BLOCK_SIZE);

}

void DataStructures::tGet(uint32_t index, unsigned char* block){
	memcpy(block, &T[index*T_BLOCK_SIZE], T_BLOCK_SIZE);
}

void DataStructures::loadFile(string filename, char* input, int64_t size){
	ifstream file(filename.c_str());
	file.seekg(0, std::ios::beg);
	file.read(input, size);
	file.close();
}

int64_t DataStructures::readFileSize(string filename){
	ifstream file(filename.c_str(), std::ios_base::binary | std::ios_base::ate);
	return file.tellg();
}
