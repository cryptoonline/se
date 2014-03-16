//
// DiskCommunicator.cpp
// BlindStorage
//

#include "DiskCommunicator.h"

// byte* DiskCommunicator::T = NULL;
// byte* DiskCommunicator::D = NULL;

DiskCommunicator::DiskCommunicator(){
//	TSize = readFileSize(T_FILE);
//	DSize = readFileSize(D_FILE);
//	
//	T = new byte[TSize];
//	D = new byte[DSize];
//
//	memset(T, 0, TSize);
//	memset(D, 0, DSize);
//	
//	cout << "Reading T from disk..." << endl;
//	loadFile(T_FILE, T, TSize);
//	cout << "T successfully read into memory." << endl;
//
//	cout << "Reading D from disk..." << endl;
//	loadFile(D_FILE, D, DSize);
//	cout << "D successfully read into memory." << endl;

//	printhex(T, TSize, "T");
//	for(int i = 0; i < TOTAL_BLOCKS; i++){
//		cout << "Block " << i << endl;
//		printhex(&D[i*BLOCK_SIZE], BLOCK_SIZE, "DiskCommunicator");
//	}
}

DiskCommunicator::~DiskCommunicator(){
//	delete[] T;
//	delete[] D;
}

void DiskCommunicator::dPut(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks){
//	boost::iostreams::mapped_file_sink file;
//	size_t DSize = (size_t)(TOTAL_BLOCKS*BLOCK_SIZE);
//	file.open(D_FILE, DSize);
//
//	if(file.is_open()){
//		byte* D = (byte*)file.data();
//				
//		for(int32_t i = 0; i < numBlocks; i++){
//			memcpy(D+blockLocations[i]*BLOCK_SIZE, &blocks[i*BLOCK_SIZE], BLOCK_SIZE);
//		}
//		file.close();
//	}
//	else{
//		cout << D_FILE << " could not be mapped." << endl;
//	}
	
	ofstream out(D_FILE, ios::out | ios::binary);
	for(int i = 0; i < numBlocks; i++){
//		memcpy(&D[blockLocations[i]*BLOCK_SIZE], &blocks[i*BLOCK_SIZE], BLOCK_SIZE);
		out.seekp(blockLocations[i]*BLOCK_SIZE);
		out.write(reinterpret_cast<char*>(&blocks[i*BLOCK_SIZE]), BLOCK_SIZE);
	}
	out.close();
}

void DiskCommunicator::dGet(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks){
	boost::iostreams::mapped_file_source file;
	size_t DSize = (size_t)(TOTAL_BLOCKS*BLOCK_SIZE);

	file.open(D_FILE, DSize);

	if(file.is_open()){
		byte* D = (byte*)file.data();
		
		for(int32_t i = 0; i < numBlocks; i++){
			memcpy(&blocks[i*BLOCK_SIZE], D+blockLocations[i]*BLOCK_SIZE, BLOCK_SIZE);
		}
		file.close();
	}
	else {
		cout << D_FILE << " could not be mapped." << endl;
	}
//	ifstream in(D_FILE, ios::in | ios::binary);
//	for(int i = 0; i < numBlocks; i++){
//		memcpy(&blocks[i*BLOCK_SIZE], &D[blockLocations[i]*BLOCK_SIZE], BLOCK_SIZE);
//		in.seekg(blockLocations[i]*BLOCK_SIZE);
//		in.read(reinterpret_cast<char*>(&blocks[i*BLOCK_SIZE]), BLOCK_SIZE);
//	}
//	in.close();
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

//	memcpy(&T[index*TBLOCK_SIZE], block, TBLOCK_SIZE);
//	ofstream out(T_FILE, ios::out | ios::binary);
//	out.seekp(index*TBLOCK_SIZE);
//	out.write(reinterpret_cast<char*>(block), TBLOCK_SIZE);
//	out.close();
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

//	memcpy(block, &T[index*TBLOCK_SIZE], TBLOCK_SIZE);
//	ifstream in(T_FILE, ios::in | ios::binary);
//	in.seekg(index*TBLOCK_SIZE);
//	in.read(reinterpret_cast<char*>(block), TBLOCK_SIZE);
//	in.close();
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

// void DiskCommunicator::writeToDisk(){
//	ofstream fileD(D_FILE, std::ios::binary);
//	fileD.write(reinterpret_cast<char*>(D), DSize);
	
//	ofstream fileT(T_FILE, std::ios::binary);
//	fileT.write(reinterpret_cast<char*>(T), TSize);

//	fileD.close();
//	fileT.close();
// }

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
