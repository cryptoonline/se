//
//  Ddisk.cpp
//  BlindStorage
//

#include "Ddisk.h"

Ddisk::Ddisk(b_index_t numBlocks){
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

void Ddisk::makeBlocks(byte bytes[], size_t size, fileID fid, vector<b_index_t>& emptyBlocks, bool isCRI){
// Bytes in this function should be interleaved for the amount of bytes needed by DataBlock trailer
	b_index_t requiredNumBlocks = (b_index_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);

	int32_t counter = 0;
	for(; counter < requiredNumBlocks - 1; counter++){
		D[emptyBlocks[counter]]->make(fid, &bytes[counter*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE, isCRI);
		if(isCRI){
			byte b[BLOCK_SIZE];
			D[emptyBlocks[counter]]->getDecrypted(b);
			printhex(b, BLOCK_SIZE, "CRI BLOCK IN DATABLOCK");
		} else{
			byte b[BLOCK_SIZE];
			D[emptyBlocks[counter]]->getDecrypted(b);
			printhex(b, BLOCK_SIZE, "DATABLOCK");
		}
	}

	dataSize_t sizeOfLastBlock = (dataSize_t)size - (dataSize_t)(size/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE;
	cout << "Last block size is " << sizeOfLastBlock << endl;
	D[emptyBlocks[counter]]->make(fid, &bytes[counter*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock, isCRI); 
	if(isCRI){
		byte b[BLOCK_SIZE];
		D[emptyBlocks[counter]]->getDecrypted(b);
		printhex(b, BLOCK_SIZE, "CRIBLOCK");
//		D[emptyBlocks[counter]]->getEncrypted(b);
//		printhex(b, BLOCK_SIZE, "CRI BLOCK IN DATABLOCK ENCRYPTED");
	} else{
		byte b[BLOCK_SIZE];
		D[emptyBlocks[counter]]->getDecrypted(b);
		printhex(b, BLOCK_SIZE, "DATABLOCK");
	}
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

void Ddisk::addFile(byte bytes[], size_t size, fileID fid, PRSubset prSubset, bool isCRI){
	if(isCRI)
		printhex(bytes, size, "CRI Bytes in Ddisk");
	vector<b_index_t> emptyBlocks;
	getEmptyBlocks(prSubset, emptyBlocks);
	if(emptyBlocks.size()*MAX_BLOCK_DATA_SIZE < size){
		cerr << "Not enough empty blocks." << endl;
		exit(1);
	}
	makeBlocks(bytes, size, fid, emptyBlocks, isCRI);
	usedBlocks += (b_index_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);
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
//		printhex(encryptedBlock, BLOCK_SIZE, __PRETTY_FUNCTION__);
	}
	cout << endl;
}
