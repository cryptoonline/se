//
//  OnlineSession.cpp
//  BlindStorage
//

#include "OnlineSession.h"

OnlineSession::OnlineSession(){
	numBlocks = 0;
	Key key(T_KEYFILE, AES_KEY_SIZE);
	byte keyBytes[AES_KEY_SIZE];
	key.get(keyBytes);
	tBlock.setKey(keyBytes);
//	this->communicator = communicator;
//	this->communicator.connect();
	
}

OnlineSession::~OnlineSession(){
}

int64_t OnlineSession::read(string filename, byte*& file, b_index_t numBlocksToRead){
	cout << "Reading " << filename << endl;
	this->filename = filename;
	fileID fid(filename);
	this->fid = fid;

	byte tBlockBytes[TBLOCK_SIZE];
	readT(fid.getHigherID(), tBlockBytes);
	TBlock tBlock(fid.getHigherID());
	tBlock.parse(tBlockBytes);
	cout << "Higher FID" << __PRETTY_FUNCTION__ << " " << fid.getHigherID() << endl;
	cout << "TBlock: Size = " << tBlock.getSize() << ", Seed = " << tBlock.getSeed() << endl;
	this->tBlock = tBlock;
	if(tBlock.getSize() == 0)
		return 0;

	PRSubset criPRSubset(tBlock.getSize(), tBlock.getSeed());
	this->criPRSubset = criPRSubset;
	
	readCRI(criPRSubset, cri);
	if(cri.isEmpty()){
		std::cerr << "CRI shouldn't be empty at this point:" << __FILE__ << ":" << __LINE__ << endl;
		exit(1);
	}

	if((criBlockIndex = cri.search(fid, criBlock)) == -1)
		return -1;

	cout << "CRIBlock: Size = " << criBlock.getSize() << ", Seed = " << criBlock.getSeed() << endl;
	numBlocks = criBlock.getSize();
	if(numBlocksToRead == 0)
		numBlocksToRead = numBlocks;
	else if(numBlocksToRead <= numBlocks)
			numBlocksToRead = numBlocks;

	numFileBlocks = numBlocks / BLOW_UP;
	
	cout << "No. of blocks " << numBlocks << " No. of file blocks " << numFileBlocks << endl;
	PRSubset filePRSubset(numBlocks, criBlock.getSeed());
	this->filePRSubset = filePRSubset;
	b_index_t blockIndices[numBlocks];
	filePRSubset.get(blockIndices, numBlocks);

	byte dataBlocksBytes[numBlocks*BLOCK_SIZE];
	readD(blockIndices, numBlocks, dataBlocksBytes);

//	printhex(dataBlocksBytes, numBlocks*BLOCK_SIZE, "BLOCK BYTES");
//	size_t size = numFileBlocks*MAX_BLOCK_DATA_SIZE;
		size_t filesize = 0;
//	int lastBlockSize = 0;
//	byte contents[size];
//	memset(contents, 0, size);
	for(int i = 0; i < numBlocks; i++){
		DataBlock block(blockIndices[i]);
		block.parse(&dataBlocksBytes[i*BLOCK_SIZE]);
//		block.getDecrypted(blockByte);
//		printhex(blockByte, BLOCK_SIZE, "BLOCK HEX");
		blocks.push_back(block);
		if(block.fidMatchCheck(fid)){
			cout << "Block Matches!" << endl;
			fileBlocks.push_back(block);
//			byte blockBytes[BLOCK_SIZE];
//			block.getDecrypted(blockBytes);
//			memcpy(&contents[i*MAX_BLOCK_DATA_SIZE], blockBytes, block.getDataSize());
			if(block.getDataSize() < MAX_BLOCK_DATA_SIZE){
//				lastBlockSize = block.getDataSize();
				filesize += block.getDataSize();
				break;
			} else {
				filesize += block.getDataSize();
			}
		}
	}
//	printchars(contents, lastBlockSize+(numFileBlocks-1)*MAX_BLOCK_DATA_SIZE, "FILE");

//	size_t filesize = (numFileBlocks-1)*MAX_BLOCK_DATA_SIZE + lastBlockSize;
	file = new byte[filesize];
	for(int i = 0; i < fileBlocks.size(); i++){
		byte block[BLOCK_SIZE];
		fileBlocks[i].getDecrypted(block);
		memcpy(&file[i*MAX_BLOCK_DATA_SIZE], block, fileBlocks[i].getDataSize());
	}

	return filesize;
//	printchars(file, (numFileBlocks-1)*MAX_BLOCK_DATA_SIZE + lastBlockSize, "FILE FROM VECTOR");
//	cout << filename << " size is " << lastBlockSize + (numFileBlocks-1)*MAX_BLOCK_DATA_SIZE << " last block size is " << lastBlockSize << endl;

//	uint32_t lastBlockSize = fileBlocks[numFileBlocks-1].getDataSize();
//	cout << "Last block size is " << lastBlockSize << endl;
//	size_t size = numFileBlocks*MAX_BLOCK_DATA_SIZE;
//	byte* contents = new byte[size]();

//	byte contents[size];
//	memset(contents, 0, size);
//	for(int i = 0; i < numFileBlocks; i++){
//		byte fbs[fileBlocks[i].getDataSize()];
//		memset(fbs, 0, fileBlocks[i].getDataSize());
//		fileBlocks[i].getDecrypted(fbs);
//		printchars(fbs, fileBlocks[i].getDataSize(), "FILE");
//		memcpy(&contents[i*MAX_BLOCK_DATA_SIZE], fbs, fileBlocks[i].getDataSize());
		//fileBlocks[i].getDecrypted(&contents[i*MAX_BLOCK_DATA_SIZE]);
		
	//	printchars(&contents[i*MAX_BLOCK_DATA_SIZE], fileBlocks[i].getDataSize(), "BLOCK CONTENTS");
//	}
//	printchars(contents, size, "FILE");
//	delete[] contents;
}

void OnlineSession::write(byte contents[], size_t size){
	byte* previous_file;
	b_index_t numBlocksToWrite = (b_index_t)ceil(((double)size/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
	size_t previous_size = read(filename, previous_file, numBlocksToWrite);

	byte lowerFid[LOWERFID_SIZE];
	fid.getLowerID(lowerFid);
	
	if(previous_size == 0){
		PRSubset filePRSubset(numBlocksToWrite*BLOW_UP);
		this->filePRSubset = filePRSubset;

		PRSubset criPRSubset(1*BLOW_UP);
		this->criPRSubset = criPRSubset;
		cri.addFile(filePRSubset.getSize(), filePRSubset.getSeed(), lowerFid);
		TBlock tBlock;
		tBlock.make(criPRSubset.getSize(), criPRSubset.getSeed());
		this->tBlock = tBlock;
	}
	else if (previous_size == -1){
		PRSubset filePRSubset(numBlocksToWrite*BLOW_UP);
		this->filePRSubset = filePRSubset;
		cri.addFile(filePRSubset.getSize(), filePRSubset.getSeed(), lowerFid);
		b_index_t criNumBlocks = ((int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
		if(criNumBlocks > criPRSubset.getSize())	
			tBlock.update(criNumBlocks, criPRSubset.getSeed());
		else
			tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
	}
	else{
		if(numBlocksToWrite > filePRSubset.getSize())
			cri.updateFile(numBlocksToWrite, filePRSubset.getSeed(), criBlockIndex);
		else
			cri.updateFile(filePRSubset.getSize(), filePRSubset.getSeed(), criBlockIndex);
	}

	//TODO: This can be optimized by making the blockIndices Array class member
	b_index_t blockIndices[filePRSubset.getSize()];
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	//TODO: Update the blocks that are free with new content
	//TODO: Update the version of the other blocks
	//TODO: Encrypt all the blocks
	//TODO: Write T
	//TODO: Write CRI
	//TODO: Write D
	
	byte blocksBytes[numBlocksToWrite*BLOCK_SIZE];
	int i = 0;
	for(; i < numBlocksToWrite - 1; ++i){
		blocks[i].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
		blocks[i].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);
	}
	dataSize_t sizeOfLastBlock = (dataSize_t)(size - (size/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE); 
	blocks[i].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
	blocks[i].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);

	
	delete[] previous_file;
}

void OnlineSession::readT(t_index_t TRecordIndex, byte block[]){
	dcomm.tGet(TRecordIndex, block);
	printhex(block, TBLOCK_SIZE, __PRETTY_FUNCTION__);
}

void OnlineSession::writeT(t_index_t TRecordIndex, byte block[]){
}

void OnlineSession::readD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]){
	dcomm.dGet(blockIndices, numBlocks, blocks);
}

void OnlineSession::writeD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]){
}

void OnlineSession::readCRI(PRSubset& prSubset, CRI& cri){
	cout << "Reading CRI!" << endl;
	b_index_t numBlocks = prSubset.getSize();
	b_index_t blockIndices[numBlocks];
	prSubset.get(blockIndices, numBlocks);
	printdec(blockIndices, numBlocks, "Indices");

	byte blocks[numBlocks*BLOCK_SIZE];
	readD(blockIndices, numBlocks, blocks);

	byte decryptedBlocks[(numBlocks/BLOW_UP)*MAX_BLOCK_DATA_SIZE];
	
	byte criFidBytes[FILEID_SIZE] = {0};
	higherfid_t higherfid = fid.getHigherID();
	memcpy(criFidBytes, static_cast<byte*>(static_cast<void*>(&higherfid)), sizeof(higherfid_t));
	fileID  criFid(criFidBytes);

	for(int i = 0; i < numBlocks; i++){
		DataBlock block(blockIndices[i]);
		block.parse(&blocks[i*BLOCK_SIZE]);
		criBlocks.push_back(block);
		if(block.fidMatchCheck(criFid))
			block.getDecrypted(&decryptedBlocks[i*MAX_BLOCK_DATA_SIZE]);
	}
	for(int i = 0; i < numBlocks; i++)
		printhex(&decryptedBlocks[i*BLOCK_SIZE], BLOCK_SIZE, "CRI BLOCKS");

	cri.parseBytes(decryptedBlocks, numBlocks*BLOCK_SIZE);
}
