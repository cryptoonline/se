//
// DiskCommunicator.cpp
// BlindStorage
//

#include "DiskCommunicator.h"

DiskCommunicator::DiskCommunicator(){
}

DiskCommunicator::~DiskCommunicator(){
}

void DiskCommunicator::dPut(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks){
	boost::iostreams::mapped_file_sink file;
	file.open(D_FILE, TOTAL_BLOCKS*BLOCK_SIZE);

	if(file.is_open()){
		byte* D = (byte*)file.data();
		
		for(int32_t i = 0; i < numBlocks; i++)
			memcpy(D+blockLocations[i]*BLOCK_SIZE, &blocks[i*BLOCK_SIZE], BLOCK_SIZE);
		
		file.close();
	}
	else{
		cout << D_FILE << " could not be mapped." << endl;
	}
}

void DiskCommunicator::dGet(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks){
	boost::iostreams::mapped_file_source file;
	file.open(D_FILE, TOTAL_BLOCKS*BLOCK_SIZE);

	if(file.is_open()){
		byte* D = (byte*)file.data();
		
		for(int32_t i = 0; i < numBlocks; i++)
			memcpy(&blocks[i*BLOCK_SIZE], D+blockLocations[i]*BLOCK_SIZE, BLOCK_SIZE);
		
		file.close();
	}
	else {
		cout << D_FILE << " could not be mapped." << endl;
	}
}

void DiskCommunicator::tPut(t_index_t index, byte* block){
	boost::iostreams::mapped_file_sink file;
	file.open(T_FILE, TTOTAL_BLOCKS*TBLOCK_SIZE);

	if(file.is_open()){
		byte* T = (byte*)file.data();
		memcpy(T+index*TBLOCK_SIZE, block, TBLOCK_SIZE);
		file.close();
	}
	else{
		cout << T_FILE << " could not be mapped." << endl;
	}
}

void DiskCommunicator::tGet(t_index_t index, byte* block){
	boost::iostreams::mapped_file_source file;
	file.open(T_FILE, TTOTAL_BLOCKS*TBLOCK_SIZE);

	if(file.is_open()){
		byte* T = (byte*)file.data();
		memcpy(block, T+index*TBLOCK_SIZE, TBLOCK_SIZE);
		file.close();
	}
	else{
		cout << T_FILE << " could not be mapped." << endl;
	}
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

void DiskCommunicator::printD(string TAG){
	for(b_index_t i = 0; i < TOTAL_BLOCKS; i++){
		cout << "**************************************************" << i << "*************************************************" << endl;;
		printhex(&D[i*BLOCK_SIZE], BLOCK_SIZE, TAG);
		printchars(&D[i*BLOCK_SIZE], BLOCK_SIZE, TAG);
	}
}

void DiskCommunicator::printT(string TAG){
	for(t_index_t i = 0; i < TTOTAL_BLOCKS; i++){
		cout << "**************************************************" << i << "*************************************************" << endl;
		printhex(&T[i*TBLOCK_SIZE], TBLOCK_SIZE, TAG);
	}
}
