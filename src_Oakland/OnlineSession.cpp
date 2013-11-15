//
//  OnlineSession.cpp
//  BlindStorage
//

#include "OnlineSession.h"

double OnlineSession::diskAccessTime = 0;

OnlineSession::OnlineSession(){
	numBlocks = 0;
	clock_t startTime = clock();
	Key key(T_KEYFILE, AES_KEY_SIZE);
	diskAccessTime += (double)(clock()-startTime)/(double)CLOCKS_PER_SEC;
	byte keyBytes[AES_KEY_SIZE];
	key.get(keyBytes);

//	Key dkey(D_KEYFILE, AES_KEY_SIZE);
//	byte DKeyBytes[AES_KEY_SIZE];
//	dkey.get(DKeyBytes);
//	blocks[0].setKey(DKeyBytes);
//	fileBlocks[0].setKey(DKeyBytes);

//	blocks.reserve(128*256);
//	fileBlocks.reserve(32*256);
//	updatedFileBlocksIndices.reserve(32*256);

	filesize = 0;
	numBlocks = 0;
//	Key localTKey(OT_KEYFILE, HMAC_KEY_SIZE);
//	byte localTKeyBytes[HMAC_KEY_SIZE];
//	key.get(localTKeyBytes);
//	localTHash.setKey(localTKeyBytes);
}

OnlineSession::~OnlineSession(){
//	writeLocalTBack();
}


void OnlineSession::firstDAccess(string filename){
	
	b_index_t blockIndices[SIZE_MIN];
	prSubset.get(blockIndices, SIZE_MIN);

	byte dataBlocksBytes[SIZE_MIN*BLOCK_SIZE];
	readD(blockIndices, SIZE_MIN, dataBlocksBytes);

//	printhex(dataBlocksBytes, SIZE_MIN*BLOCK_SIZE, "DATA BYTES");

//	size_t filesize;

	bool fileBlockExists = false;

	for(int i = 0; i < SIZE_MIN; i++){
		/* After reading the final file block i.e. one that have data less than MAX_BLOCK_DATA_SIZE the loop can be broken
		 * but only for read. For update we need to process all the blocks */
		DataBlock block(blockIndices[i]);
		block.parse(&dataBlocksBytes[i*BLOCK_SIZE]);
		blocks.push_back(block);
		if(block.fidMatchCheck(fid)){
			if(!fileBlockExists){
//				printhex(&dataBlocksBytes[i*BLOCK_SIZE], BLOCK_SIZE, "FIRST BLOCK");
				filesize = *(size_t*)(&dataBlocksBytes[i*BLOCK_SIZE]);
				numBlocks = (b_index_t)(ceil((double)filesize/(double)MAX_BLOCK_DATA_SIZE));
				fileBlockExists = true;
			}
			fileBlocks.push_back(block);
			updatedFileBlocksIndices.push_back(i);
			}
		}
}

void OnlineSession::secondDAccess(string filename){


	b_index_t* blockIndices = new b_index_t[numBlocks*BLOW_UP];
	prSubset.get(blockIndices, numBlocks*BLOW_UP);

	byte* dataBlocksBytes = new byte[numBlocks*BLOW_UP*BLOCK_SIZE];
	readD(blockIndices, numBlocks*BLOW_UP, dataBlocksBytes);

//	size_t filesize;

	for(int i = 0; i < numBlocks*BLOW_UP - SIZE_MIN; i++){
		/* After reading the final file block i.e. one that have data less than MAX_BLOCK_DATA_SIZE the loop can be broken
		 * but only for read. For update we need to process all the blocks */
		DataBlock block(blockIndices[i+SIZE_MIN]);
		block.parse(&dataBlocksBytes[(i+SIZE_MIN)*BLOCK_SIZE]);
		blocks.push_back(block);
		if(block.fidMatchCheck(fid)){
			fileBlocks.push_back(block);
			updatedFileBlocksIndices.push_back(i+SIZE_MIN);
			}
		}

	delete[] blockIndices;
	delete[] dataBlocksBytes;
}

/*
size_t OnlineSession::updateReadWithLocalT(string filename, byte*& file, size_t updatedFileSize){
	bool file_present = readOT(filename, this->filePRSubset);

	if(!file_present){
		PRSubset filePRSubset(updatedFileSize);
		this->filePRSubset = filePRSubset;
	}

	b_index_t blockIndices[filePRSubset.getSize()];
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	byte dataBlocksBytes[filePRSubset.getSize()*BLOCK_SIZE];
	readD(blockIndices, filePRSubset.getSize(), dataBlocksBytes);

	uint32_t numBlocks = filePRSubset.getSize();

	size_t filesize = 0;
	for(int i = 0; i < numBlocks; i++){
		// After reading the final file block i.e. one that have data less than MAX_BLOCK_DATA_SIZE the loop can be broken
//		 * but only for read. For update we need to process all the blocks 
		DataBlock block(blockIndices[i]);
		block.parse(&dataBlocksBytes[i*BLOCK_SIZE]);
		blocks.push_back(block);
		if(block.fidMatchCheck(fid)){
			fileBlocks.push_back(block);
			updatedFileBlocksIndices.push_back(i);
			filesize += block.getDataSize();
			}
		}
	return filesize;	
}

void OnlineSession::updateWriteWithLocalT(string filename, byte updatedFile[], size_t updatedFileSize){
	b_index_t numBlocksToWrite = (b_index_t)(ceil((double)updatedFileSize/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
	dataSize_t sizeOfLastBlock = (dataSize_t)(updatedFileSize - (updatedFileSize/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE);

	
	b_index_t blockIndices[filePRSubset.getSize()];	
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	byte blocksBytes[filePRSubset.getSize()*BLOCK_SIZE];
	memset(blocksBytes, 0, filePRSubset.getSize()*BLOCK_SIZE);

	int j = 0;
	for(int i = 0; i < blocks.size(); ++i){
		
		if(i == updatedFileBlocksIndices[j]){
			if(j < (numBlocksToWrite/BLOW_UP) - 1)
				blocks[i].update(fid, &updatedFile[j*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
			else if( j == numBlocksToWrite/BLOW_UP - 1)
				blocks[i].update(fid, &updatedFile[j*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);				
			j++;
		}
		else
			blocks[i].updateVersion();
		blocks[i].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);
	}
	
	writeOT(filename, filePRSubset);
	writeD(blockIndices, filePRSubset.getSize(), blocksBytes);
}
*/

size_t OnlineSession::updateRead(string filename, byte*& file, size_t bytesToAdd){
	
	this->filename = filename;
	fileID fid(filename);
	this->fid = fid;

	byte lowerFid[LOWERFID_SIZE];
	this->fid.getLowerID(lowerFid);
//	printhex(lowerFid, LOWERFID_SIZE, "LOWER FID");

	PRSubset prSubset(SIZE_MIN, filename);
	this->prSubset = prSubset;

	firstDAccess(filename);
	b_index_t numBlocksAfterUpdate = (b_index_t)(ceil((double)(filesize+bytesToAdd)/(double)MAX_BLOCK_DATA_SIZE));
	numBlocks = max(numBlocks, numBlocksAfterUpdate);
	if(numBlocks*BLOW_UP > SIZE_MIN){
		secondDAccess(filename);
	}
	else
		numBlocks = SIZE_MIN;

	file = new byte[filesize];
	memset(file, 0, filesize);
	for(int i = 1; i < fileBlocks.size(); i++){
		byte block[BLOCK_SIZE];
		memset(block, 0, BLOCK_SIZE);
		fileBlocks[i].getDecrypted(block);
		memcpy(&file[(i-1)*MAX_BLOCK_DATA_SIZE], block, fileBlocks[i].getDataSize());
	}

	return filesize;
}

void OnlineSession::updateWrite(string filename, byte updatedFile[], size_t sizeAfterUpdate){
	b_index_t* blockIndices = new b_index_t[numBlocks*BLOW_UP];	
	prSubset.get(blockIndices, numBlocks*BLOW_UP);

	if(updatedFileBlocksIndices.size() > numBlocks)
		for(int i = numBlocks; i < updatedFileBlocksIndices.size(); i++)
			blocks[updatedFileBlocksIndices[i]].clear();

	for(int i = 0; i < blocks.size() && updatedFileBlocksIndices.size() < numBlocks; i++)
		if(!(blocks[i].isOccupied()))
			updatedFileBlocksIndices.push_back(i);

	if(updatedFileBlocksIndices.size() < numBlocks){
		cerr << "Update unsuccessful: Not enought blocks available." << endl;
		exit(1);
	}

	byte* blocksBytes = new byte[numBlocks*BLOW_UP*BLOCK_SIZE];
	memset(blocksBytes, 0, numBlocks*BLOW_UP*BLOCK_SIZE);
	
	byte sizef[sizeof(size_t)];
	memset(sizef, 0, sizeof(size_t));
//	size_t maxsize = max(filesize, sizeAfterUpdate);
	memcpy(sizef, reinterpret_cast<byte*>(&sizeAfterUpdate), sizeof(size_t));
	
	blocks[0].update(fid, sizef, sizeof(size_t));
	blocks[0].getEncrypted(&blocksBytes[0]);

	int i = 1, j = 1;

//	printhex(blocksBytes, numBlocks, "CHARS");

	dataSize_t sizeOfLastBlock = (dataSize_t)(sizeAfterUpdate - (sizeAfterUpdate/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE);
	for(; i < blocks.size(); ++i){
		
		if(i == updatedFileBlocksIndices[j]){
			if(j < numBlocks - 1)
				blocks[i].update(fid, &updatedFile[j*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
			else if( j == numBlocks - 1)
				blocks[i].update(fid, &updatedFile[j*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);				
			j++;
		}
		else
			blocks[i].updateVersion();
		blocks[i].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);
	}

//	printhex(blocksBytes, numBlocks*BLOW_UP*BLOCK_SIZE, "CHARS");
	writeD(blockIndices, numBlocks, blocksBytes);
	
	delete[] blocksBytes;	
	delete[] blockIndices;
//	dcomm.writeToDisk();
}

/*bool OnlineSession::readOT(string filename, PRSubset &prSubset){
	byte mac[SHA_BLOCK_SIZE];
	localTHash.doFinal(filename, mac);
	
	uint64_t pairkey = *(uint64_t*)mac;

	std::unordered_map<uint64_t, PRSubset>::const_iterator got = localT.find(pairkey);

	if(got == localT.end())
		return false;
	else{
		prSubset = got->second;
		return true;
	}

//	prSubset = localT.at(pairkey);
}

void OnlineSession::writeOT(string filename, PRSubset prSubset){
	byte mac[SHA_BLOCK_SIZE];
	localTHash.doFinal(filename, mac);

	uint64_t pairkey = *(uint64_t*)mac;

	localT.insert(std::make_pair<uint64_t, PRSubset>(pairkey, prSubset));
}
*/

void OnlineSession::readD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]){
	clock_t startTime = clock();
	dcomm.dGet(blockIndices, numBlocks, blocks);
	diskAccessTime += (double)(clock() - startTime)/(double)CLOCKS_PER_SEC;
}

void OnlineSession::writeD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]){
	clock_t startTime = clock();
	dcomm.dPut(blockIndices, numBlocks, blocks);
	diskAccessTime += (double)(clock() - startTime)/(double)CLOCKS_PER_SEC;
}

void OnlineSession::resetDiskAccessTime(){
	diskAccessTime = 0;
}

double OnlineSession::getDiskAccessTime(){
	return diskAccessTime;
}

/*
void OnlineSession::loadLocalT(){
	int buffersize = sizeof(uint64_t)+sizeof(prSubsetSize_t)+sizeof(prSubsetSeed_t);
	char buffer[buffersize];

	size_t localTSize = readFileSize(OT_FILE);

	byte* localTBytes = new byte[localTSize];
	readFile(OT_FILE, localTBytes,localTSize);
	
	for(int i = 0; i < localTSize/buffersize; i++){
		uint64_t pairkey = *(uint64_t*)(localTBytes+i*buffersize);		prSubsetSize_t size = *(prSubsetSize_t*)(localTBytes+i*buffersize+sizeof(uint64_t));
		prSubsetSeed_t seed = *(prSubsetSeed_t*)(localTBytes+i*buffersize+sizeof(uint64_t)+sizeof(prSubsetSize_t));

		PRSubset prSubset(size, seed);
		localT.insert(std::make_pair<uint64_t, PRSubset>(pairkey, prSubset));
	}

	delete[] localTBytes;
}

void OnlineSession::writeLocalTBack(){
	ofstream file(OT_FILE, std::ios::out | std::ios::binary);
	char buffer[sizeof(uint64_t)+sizeof(prSubsetSize_t)+sizeof(prSubsetSeed_t)];
	cout << "Writing back T to local disk. Please wait!" << endl;
	for(unordered_map<uint64_t, PRSubset>::iterator iter=localT.begin(); iter!=localT.end(); ++iter){
		uint64_t key = iter->first;
		PRSubset prSubset = iter->second;

		prSubsetSize_t size = prSubset.getSize();
		prSubsetSeed_t seed = prSubset.getSeed();

		file.write(reinterpret_cast<char*>(&key), sizeof(uint16_t));
		file.write(reinterpret_cast<char*>(&size), sizeof(prSubsetSize_t));
		file.write(reinterpret_cast<char*>(&seed), sizeof(prSubsetSeed_t));
	}
	file.close();
	cout << "T written back to local disk." << endl;
}
*/
