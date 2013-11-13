//
//  OnlineSession.cpp
//  BlindStorage
//

#include "OnlineSession.h"

double OnlineSession::diskAccessTime = 0;

OnlineSession::OnlineSession(){
	numBlocks = 0;
	previousCriSize = 0;
	clock_t startTime = clock();
	Key key(T_KEYFILE, AES_KEY_SIZE);
	diskAccessTime += (double)(clock()-startTime)/(double)CLOCKS_PER_SEC;
	byte keyBytes[AES_KEY_SIZE];
	key.get(keyBytes);
	tBlock.setKey(keyBytes);

//	Key dkey(D_KEYFILE, AES_KEY_SIZE);
//	byte DKeyBytes[AES_KEY_SIZE];
//	dkey.get(DKeyBytes);
//	blocks[0].setKey(DKeyBytes);
//	fileBlocks[0].setKey(DKeyBytes);

	blocks.reserve(128);
	fileBlocks.reserve(32);
	updatedFileBlocksIndices.reserve(32);

	criBlocks.reserve(128);
	fileCriBlocks.reserve(32);
	updatedCriFileBlocksIndices.reserve(32);

	Key localTKey(OT_KEYFILE, HMAC_KEY_SIZE);
	byte localTKeyBytes[HMAC_KEY_SIZE];
	key.get(localTKeyBytes);
	localTHash.setKey(localTKeyBytes);
}

OnlineSession::~OnlineSession(){
	writeLocalTBack();
}

int OnlineSession::retrieveTBlock(){
	byte tBlockBytes[TBLOCK_SIZE];
	readT(fid.getHigherID(), tBlockBytes);
	TBlock tBlock(fid.getHigherID());

	tBlock.parse(tBlockBytes);
	this->tBlock = tBlock;
	if(tBlock.getSize() == 0)
		return 0;
	return 1;
}

size_t OnlineSession::retrieveDBlocks(b_index_t numBlocksToWrite){
//	size_t filesize = 0;
	
	numBlocks = max(numBlocksToWrite, criBlock.getSize());

	numFileBlocks = numBlocks / BLOW_UP;
	
	PRSubset filePRSubset(numBlocks, criBlock.getSeed());
	this->filePRSubset = filePRSubset;
	b_index_t blockIndices[filePRSubset.getSize()];
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	byte dataBlocksBytes[numBlocks*BLOCK_SIZE];
	readD(blockIndices, numBlocks, dataBlocksBytes);

	for(int i = 0; i < numBlocks; i++){
		/* After reading the final file block i.e. one that have data less than MAX_BLOCK_DATA_SIZE the loop can be broken
		 * but only for read. For update we need to process all the blocks */
		DataBlock block(blockIndices[i]);
		block.parse(&dataBlocksBytes[i*BLOCK_SIZE]);
		blocks.push_back(block);
		if(block.fidMatchCheck(fid)){
			fileBlocks.push_back(block);
			updatedFileBlocksIndices.push_back(i);
//			filesize += block.getDataSize();
			}
		}

	return criBlock.getFileSize();
}

void extractFileBlocks(){
	
}

int OnlineSession::retrieveCRIBlock(){
	PRSubset criPRSubset(tBlock.getSize(), tBlock.getSeed());
	this->criPRSubset = criPRSubset;
	
	readCRI(criPRSubset, cri);
	
	if(cri.isEmpty()){
		return 0;
//		std::cerr << "CRI shouldn't be empty at this point:" << __FILE__ << ":" << __LINE__ << endl;
//		exit(1);	
	}

	criBlockIndex = cri.search(fid, criBlock);
	if(criBlockIndex == -1)
		return 0;
	return 1;
}

size_t OnlineSession::read(string filename, byte*& file, b_index_t numBlocksToRead){

	this->filename = filename;
	fileID fid(filename);
	this->fid = fid;
	
	retrieveTBlock();
	retrieveCRIBlock();
	
	size_t filesize = retrieveDBlocks();
	
	file = new byte[filesize];

	memset(file, 0, filesize);
	for(int i = 0; i < fileBlocks.size(); i++){
		byte block[BLOCK_SIZE];
		memset(block, 0, BLOCK_SIZE);
		fileBlocks[i].getDecrypted(block);
		memcpy(&file[i*MAX_BLOCK_DATA_SIZE], block, fileBlocks[i].getDataSize());
	}

	return filesize;
//	if(compFile[0] == 1){
//	LZO decompressor;
//	uint32_t decompressedSize = *(uint32_t*)(compFile); 
//	cout << "Decompressed Size is " << decompressedSize << endl;
//	file = new byte[decompressedSize];
//	size_t decompressedSizeFromLZO;

//	decompressor.decompress(compFile, compFilesize, file, decompressedSizeFromLZO);
//	delete[] compFile;
//	return decompressedSize;
//	}
//	else{
//		file = compFile;
//		return compFilesize;
//	}
}

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
		/* After reading the final file block i.e. one that have data less than MAX_BLOCK_DATA_SIZE the loop can be broken
		 * but only for read. For update we need to process all the blocks */
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

size_t OnlineSession::updateRead(string filename, byte*& file, int64_t bytesToAdd){
	b_index_t numBlocksToWrite = 0;
	if(bytesToAdd >= 0)
		numBlocksToWrite = (b_index_t)(ceil((double)bytesToAdd/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
	
	this->filename = filename;
	fileID fid(filename);
	this->fid = fid;

	byte lowerFid[LOWERFID_SIZE];
	this->fid.getLowerID(lowerFid);

	size_t oldFileSize = 0;

	if(retrieveTBlock() == 0){
		if(bytesToAdd < 0){
			std::cerr << "Error: Reducing filesize of the file that doesn't exists in BStore. (TBLOCK NOT FOUND)" << endl;
			exit(1);
		}

		PRSubset filePRSubset(numBlocksToWrite);
		this->filePRSubset = filePRSubset;

		PRSubset criPRSubset(1*BLOW_UP);
		this->criPRSubset = criPRSubset;

		readCRI(this->criPRSubset, this->cri);
		criBlock.make(filePRSubset.getSize(), filePRSubset.getSeed(), bytesToAdd, lowerFid);	
		cri.addFile(filePRSubset.getSize(), filePRSubset.getSeed(), bytesToAdd, lowerFid);
		
		TBlock tBlock(fid.getHigherID());
		tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
		this->tBlock = tBlock;
	
		retrieveDBlocks(filePRSubset.getSize());
	}
	else if(retrieveCRIBlock() == 0){
		if(bytesToAdd < 0){
			std::cerr << "Error: Reducing filesize of the file that doesn't exists in BStore. (TBLOCK FOUND BUT CRI NOT FOUND)" << endl;
			exit(1);
		}

		PRSubset filePRSubset(numBlocksToWrite);
		this->filePRSubset = filePRSubset;
		criBlock.make(filePRSubset.getSize(), filePRSubset.getSeed(), bytesToAdd, lowerFid);
		cri.addFile(filePRSubset.getSize(), filePRSubset.getSeed(), bytesToAdd, lowerFid);
		b_index_t criNumBlocks = (b_index_t)(ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
		if(criNumBlocks > criPRSubset.getSize()){	
			PRSubset criPRSubset(criNumBlocks, this->criPRSubset.getSeed());
			this->criPRSubset = criPRSubset;
			tBlock.update(this->criPRSubset.getSize(), this->criPRSubset.getSeed());
		}
		else
			tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
		retrieveDBlocks(filePRSubset.getSize());
	}
	else{
		oldFileSize = criBlock.getFileSize();
		size_t updatedFileSize = criBlock.getFileSize() + bytesToAdd;
		
		if(bytesToAdd < 0)
			numBlocksToWrite = (b_index_t)(ceil((double)oldFileSize/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
		else
			numBlocksToWrite = (b_index_t)(ceil((double)updatedFileSize/(double)MAX_BLOCK_DATA_SIZE))*BLOW_UP;

		PRSubset filePRSubset(numBlocksToWrite, criBlock.getSeed());
		this->filePRSubset = filePRSubset;
		cri.updateFile(filePRSubset.getSize(), filePRSubset.getSeed(), updatedFileSize, criBlockIndex);
		criBlock.make(filePRSubset.getSize(), filePRSubset.getSeed(), updatedFileSize, lowerFid);
		tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
		retrieveDBlocks(filePRSubset.getSize());
//		cout << "No. of blocks retrieved are " << filePRSubset.getSize() << endl; 
//		cout << "No. of blocks are " << criBlock.getSize() << endl;
	}
	
	file = new byte[oldFileSize];
	memset(file, 0, oldFileSize);
	for(int i = 0; i < fileBlocks.size(); i++){
		byte block[BLOCK_SIZE];
		memset(block, 0, BLOCK_SIZE);
		fileBlocks[i].getDecrypted(block);
		memcpy(&file[i*MAX_BLOCK_DATA_SIZE], block, fileBlocks[i].getDataSize());
	}

	return oldFileSize;
}

void OnlineSession::updateWrite(string filename, byte updatedFile[], size_t updatedFileSize){
	b_index_t numBlocksToWrite = (b_index_t)(ceil((double)updatedFileSize/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);

	b_index_t blockIndices[filePRSubset.getSize()];	
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	byte blocksBytes[filePRSubset.getSize()*BLOCK_SIZE];
	memset(blocksBytes, 0, filePRSubset.getSize()*BLOCK_SIZE);
		
	if(updatedFileBlocksIndices.size() > numBlocksToWrite/BLOW_UP)
		for(int i = numBlocksToWrite/BLOW_UP; i < updatedFileBlocksIndices.size(); i++)
			blocks[updatedFileBlocksIndices[i]].clear();

	for(int i = 0; i < blocks.size() && updatedFileBlocksIndices.size() < numBlocksToWrite/BLOW_UP; i++)
		if(!(blocks[i].isOccupied()))
			updatedFileBlocksIndices.push_back(i);

	if(updatedFileBlocksIndices.size() < numBlocksToWrite/BLOW_UP){
		cerr << "Update unsuccessful: Not enought blocks available." << endl;
		exit(1);
	}

	int i = 0;	int j = 0;
	dataSize_t sizeOfLastBlock = (dataSize_t)(updatedFileSize - (updatedFileSize/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE);
	for(; i < blocks.size(); ++i){
		
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
	
	
	byte tBlockBytes[TBLOCK_SIZE];
	tBlock.getEncrypted(tBlockBytes);
	
	writeT(fid.getHigherID(), tBlockBytes);
	writeD(blockIndices, filePRSubset.getSize(), blocksBytes);
	writeCRI();	
	
//	dcomm.writeToDisk();
}

void OnlineSession::write(string filename, byte contents[], size_t size){
	b_index_t numBlocksToWrite = (b_index_t)(ceil((double)size/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
	this->filename = filename;
	fileID fid(filename);
	this->fid = fid;

	byte lowerFid[LOWERFID_SIZE];
	this->fid.getLowerID(lowerFid);

	if(retrieveTBlock() == 0){
		PRSubset filePRSubset(numBlocksToWrite);
		this->filePRSubset = filePRSubset;

		PRSubset criPRSubset(1*BLOW_UP);
		this->criPRSubset = criPRSubset;

		readCRI(this->criPRSubset, this->cri);
		criBlock.make(filePRSubset.getSize(), filePRSubset.getSeed(), size, lowerFid);	
		cri.addFile(filePRSubset.getSize(), filePRSubset.getSeed(), size, lowerFid);
		
		TBlock tBlock(fid.getHigherID());
		tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
		this->tBlock = tBlock;
	
		retrieveDBlocks(filePRSubset.getSize());
	}
	else if(retrieveCRIBlock() == 0){
		PRSubset filePRSubset(numBlocksToWrite);
		this->filePRSubset = filePRSubset;
		criBlock.make(filePRSubset.getSize(), filePRSubset.getSeed(), size, lowerFid);
		cri.addFile(filePRSubset.getSize(), filePRSubset.getSeed(), size, lowerFid);
		b_index_t criNumBlocks = (b_index_t)(ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
		if(criNumBlocks > criPRSubset.getSize()){	
			PRSubset criPRSubset(criNumBlocks, this->criPRSubset.getSeed());
			this->criPRSubset = criPRSubset;
			tBlock.update(this->criPRSubset.getSize(), this->criPRSubset.getSeed());
		}
		else
			tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
		retrieveDBlocks(filePRSubset.getSize());
	}
	else{
		b_index_t maxNumBlocks = max(numBlocksToWrite, criBlock.getSize());
		PRSubset filePRSubset(maxNumBlocks, criBlock.getSeed());
		this->filePRSubset = filePRSubset;
		cri.updateFile(filePRSubset.getSize(), filePRSubset.getSeed(), size, criBlockIndex);
		criBlock.make(filePRSubset.getSize(), filePRSubset.getSeed(), size, lowerFid);
		tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
		retrieveDBlocks(filePRSubset.getSize());
	}

	//TODO: This can be optimized by making the blockIndices Array class member
	b_index_t blockIndices[filePRSubset.getSize()];
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	byte blocksBytes[filePRSubset.getSize()*BLOCK_SIZE];
	memset(blocksBytes, 0, filePRSubset.getSize()*BLOCK_SIZE);
		
	if(updatedFileBlocksIndices.size() > numBlocksToWrite/BLOW_UP)
		for(int i = numBlocksToWrite/BLOW_UP; i < updatedFileBlocksIndices.size(); i++)
			blocks[updatedFileBlocksIndices[i]].clear();

	for(int i = 0; i < blocks.size() && updatedFileBlocksIndices.size() < numBlocksToWrite/BLOW_UP; i++){
		if(!(blocks[i].isOccupied()))
			updatedFileBlocksIndices.push_back(i);
	}


	if(updatedFileBlocksIndices.size() < numBlocksToWrite/BLOW_UP){
		cerr << "Update unsuccessful: Not enought blocks available." << endl;
		exit(1);
	}

	int i = 0;
	int j = 0;
	dataSize_t sizeOfLastBlock = (dataSize_t)(size - (size/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE);
	for(; i < blocks.size(); ++i){
		
		if(i == updatedFileBlocksIndices[j]){
			if(j < (numBlocksToWrite/BLOW_UP) - 1)
				blocks[i].update(fid, &contents[j*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
			else if( j == numBlocksToWrite/BLOW_UP - 1)
				blocks[i].update(fid, &contents[j*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);				
			j++;
		}
		else
			blocks[i].updateVersion();
		blocks[i].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);
	}
	
	
	byte tBlockBytes[TBLOCK_SIZE];
	tBlock.getEncrypted(tBlockBytes);
	
	writeT(fid.getHigherID(), tBlockBytes);
	writeD(blockIndices, filePRSubset.getSize(), blocksBytes);
	writeCRI();	

//	dcomm.writeToDisk();
}

void OnlineSession::remove(string filename){
	this->filename = filename;
	fileID fid(filename);
	this->fid = fid;

	if(retrieveTBlock() == 0){
//		cout << "File not found." << endl;
	}
	else if(retrieveCRIBlock() == 0){
//		cout << "File not found." << endl;
	}
	else{
		retrieveDBlocks();

		cri.updateFile(0, 0, 0, criBlockIndex);
		if(cri.size() == 0)
			tBlock.update(0,0);
		else
			tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
		
		byte blockBytes[filePRSubset.getSize()*BLOCK_SIZE];
		memset(blockBytes, 0, filePRSubset.getSize()*BLOCK_SIZE);

		for(int i = 0; i < blocks.size(); i++){
			byte decryptedBlock[BLOCK_SIZE];
			blocks[i].getDecrypted(decryptedBlock);
			if(blocks[i].fidMatchCheck(this->fid)){
				blocks[i].clear();
			}
			blocks[i].updateVersion();
			blocks[i].getEncrypted(&blockBytes[i*BLOCK_SIZE]);
			byte decryptedBlockAfter[BLOCK_SIZE];
			blocks[i].getDecrypted(decryptedBlockAfter);
		}
		b_index_t blockIndices[filePRSubset.getSize()];
		filePRSubset.get(blockIndices, filePRSubset.getSize());


		byte tBlockBytes[TBLOCK_SIZE];
		tBlock.getEncrypted(tBlockBytes);
		
		writeT(fid.getHigherID(), tBlockBytes);
		writeD(blockIndices, filePRSubset.getSize(), blockBytes);
		writeCRI();
//		dcomm.writeToDisk();
	}
}

void OnlineSession::readT(t_index_t TRecordIndex, byte block[]){
	clock_t startTime = clock();
	dcomm.tGet(TRecordIndex, block);
	diskAccessTime += (double)(clock()-startTime)/(double)CLOCKS_PER_SEC;
}

bool OnlineSession::readOT(string filename, PRSubset &prSubset){
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

void OnlineSession::writeT(t_index_t TRecordIndex, byte block[]){
	clock_t startTime = clock();
	dcomm.tPut(TRecordIndex, block);
	diskAccessTime += (double)(clock() - startTime)/(double)CLOCKS_PER_SEC;
}

void OnlineSession::writeOT(string filename, PRSubset prSubset){
	byte mac[SHA_BLOCK_SIZE];
	localTHash.doFinal(filename, mac);

	uint64_t pairkey = *(uint64_t*)mac;

	localT.insert(std::make_pair<uint64_t, PRSubset>(pairkey, prSubset));
}

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

void OnlineSession::readCRI(PRSubset& prSubset, CRI& cri){

	/* Request additional BLOW_UP number of blocks in case the CRI requires more blocks. */
	b_index_t numBlocks = prSubset.getSize();
	b_index_t blockIndices[numBlocks];

//	cout << "Num blocks are " << numBlocks << endl;
	
	prSubset.get(blockIndices, numBlocks);

	byte blocks[numBlocks*BLOCK_SIZE];
	readD(blockIndices, numBlocks, blocks);

	byte decryptedBlocks[numBlocks*MAX_BLOCK_DATA_SIZE];
	memset(decryptedBlocks, 0, numBlocks*MAX_BLOCK_DATA_SIZE);

	byte criFidBytes[FILEID_SIZE] = {0};
	higherfid_t higherfid = fid.getHigherID();
	memcpy(criFidBytes, static_cast<byte*>(static_cast<void*>(&higherfid)), sizeof(higherfid_t));
	fileID criFid(criFidBytes);
	this->criFid = criFid;

	uint32_t criSize = 0;
	int j = 0;
	for(int i = 0; i < numBlocks; i++){
		DataBlock block(blockIndices[i]);
		block.parse(&blocks[i*BLOCK_SIZE]);
		criBlocks.push_back(block);
		if(block.fidMatchCheck(criFid)){
			block.getDecrypted(&decryptedBlocks[j*MAX_BLOCK_DATA_SIZE]);
			fileCriBlocks.push_back(block);
			updatedCriFileBlocksIndices.push_back(i);
			criSize += block.getDataSize();
			j++;
		}
	}
//	for(int i = 0; i < numBlocks; i++)
//		printhex(&decryptedBlocks[i*BLOCK_SIZE], BLOCK_SIZE, "CRI BLOCKS");

	cri.parseBytes(decryptedBlocks, criSize);
}

void OnlineSession::writeCRI(){
	uint32_t numBlocks = (uint32_t)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE);
	byte criBytes[numBlocks*MAX_BLOCK_DATA_SIZE];
	memset(criBytes, 0, numBlocks*MAX_BLOCK_DATA_SIZE);
	cri.makeBytes(criBytes);

	b_index_t criBlocksIndices[criPRSubset.getSize()];
	criPRSubset.get(criBlocksIndices, criPRSubset.getSize());

	byte encryptedBlocks[criPRSubset.getSize()*BLOCK_SIZE];
	memset(encryptedBlocks, 0, criPRSubset.getSize()*BLOCK_SIZE);

	for(int i = 0; i < criBlocks.size() && updatedCriFileBlocksIndices.size() < numBlocks; i++)
		if(!(criBlocks[i].isOccupied()))
			updatedCriFileBlocksIndices.push_back(i);

	int i = 0;
	int j = 0;
	for(; i < criBlocks.size() - 1; i++){
		if( i == updatedCriFileBlocksIndices[j]){
			criBlocks[i].update(criFid, &criBytes[j*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE, true);
			j++;
		}
		else
			criBlocks[i].updateVersion();
		criBlocks[i].getEncrypted(&encryptedBlocks[i*BLOCK_SIZE]);
	}
	dataSize_t sizeOfLastBlock = (dataSize_t)ceil(cri.size()-(cri.size()/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE);
	if( i == updatedCriFileBlocksIndices[i] ){
		criBlocks[i].update(criFid, &criBytes[j*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock, true);
		j++;
	}
	else
		criBlocks[i].updateVersion();
	criBlocks[i].getEncrypted(&encryptedBlocks[i*BLOCK_SIZE]);

	writeD(criBlocksIndices, criPRSubset.getSize(), encryptedBlocks);
}

void OnlineSession::resetDiskAccessTime(){
	diskAccessTime = 0;
}

double OnlineSession::getDiskAccessTime(){
	return diskAccessTime;
}

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
