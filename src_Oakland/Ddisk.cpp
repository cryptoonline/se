//
//  Ddisk.cpp
//  BlindStorage
//

#include "Ddisk.h"

Ddisk::Ddisk(b_index_t numBlocks){
	numOccupiedBlocks = 0;
	this->numBlocks = numBlocks;
	usedBlocks = 0;

	D = new DataBlock*[TOTAL_BLOCKS];
	for(b_index_t i = 0; i < TOTAL_BLOCKS; i++)
		D[i] = new DataBlock(i);

	Key key(D_KEYFILE, AES_KEY_SIZE);
	byte keyBytes[AES_KEY_SIZE];
	key.get(keyBytes);
	
	D[0]->setKey(keyBytes);

}

Ddisk::~Ddisk(){
	delete[] D;
//	delete[] zeroBytes;
}

void Ddisk::makeBlocks(byte bytes[], size_t size, fileID fid, vector<b_index_t>& emptyBlocks){
// Bytes in this function should be interleaved for the amount of bytes needed by DataBlock trailer
	b_index_t requiredNumBlocks = (b_index_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);
	numOccupiedBlocks += requiredNumBlocks;


	byte sizef[sizeof(size_t)];
	memset(sizef, 0, sizeof(size_t));
	memcpy(sizef, static_cast<byte*>(static_cast<void*>(&size)), sizeof(size_t));
//	cout << "sizef is " << size << " " << (int)sizef[0] << " " << (int)sizef[1] << " " << (int)sizef[2] << " " << (int)sizef[3] << endl;
//	cout << *(size_t*)sizef << endl;

	D[emptyBlocks[0]]->make(fid, sizef, sizeof(size_t));
	
	int32_t counter = 1;
	for(; counter < requiredNumBlocks; counter++){
		D[emptyBlocks[counter]]->make(fid, &bytes[(counter-1)*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
	}

	dataSize_t sizeOfLastBlock = (dataSize_t)(size - (size/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE);
//	dataSize_t sizeOfLastBlock = (dataSize_t)size - (dataSize_t)(size/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE;
	D[emptyBlocks[counter]]->make(fid, &bytes[(counter-1)*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
	
}

void Ddisk::getEmptyBlocks(PRSubset prSubset, vector<b_index_t>& emptyBlocks){
	prSubsetSize_t size = prSubset.getSize();
	b_index_t subset[size];
	prSubset.get(subset, size);

	for(int i = 0; i < size; i++){
		if(!(D[subset[i]]->isOccupied()))
			emptyBlocks.push_back(subset[i]);
	}
}

void Ddisk::addFile(byte bytes[], size_t size, fileID fid, PRSubset prSubset){
	vector<b_index_t> emptyBlocks;
//	emptyBlocks.reserve(64);

	getEmptyBlocks(prSubset, emptyBlocks);
	if(emptyBlocks.size() < (uint32_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE)){
		cerr << "Not enough empty blocks." << endl;
		exit(1);
	}
	makeBlocks(bytes, size, fid, emptyBlocks);
//	usedBlocks += (b_index_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);
}

void Ddisk::getBlock(b_index_t index, DataBlock& block){
	block = *D[index];
}

void Ddisk::encryptEmptyBlocks(){
//	size_t numZeroBytes = (numBlocks-usedBlocks)*BLOCK_SIZE;
//	zeroBytes = new byte[numZeroBytes];
//	memset(zeroBytes, 0, numZeroBytes); 
//	int j = 0;
	for(int i = 0; i < numBlocks; i++){
		if(!D[i]->isOccupied()){
			D[i]->encryptIfEmpty();//&zeroBytes[j*BLOCK_SIZE]);
//			j++;
		}
	}
}

void Ddisk::writeToDisk(){
	ofstream file(D_FILE, std::ios::binary);

	for(int i = 0; i < numBlocks; i++){
		cout << (i+1)*100/numBlocks << "\% blocks processed.";
		cout.flush();
		cout << "\r";
		byte encryptedBlock[BLOCK_SIZE] = {0};
		D[i]->getEncrypted(encryptedBlock);
		file.write(reinterpret_cast<char*>(encryptedBlock), BLOCK_SIZE);
	}
	cout << endl;
}

b_index_t Ddisk::getNumOccupiedBlocks(){
	return numOccupiedBlocks;
}
