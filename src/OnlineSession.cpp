//
//  OnlineSession.cpp
//  BlindStorage
//

#include "OnlineSession.h"

OnlineSession::OnlineSession(){
	numBlocks = 0;
//	this->communicator = communicator;
//	this->communicator.connect();
	
}

OnlineSession::~OnlineSession(){
}

bool OnlineSession::read(string filename){
	cout << "Reading " << filename << endl;
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
		return false;

	PRSubset criPRSubset(tBlock.getSize(), tBlock.getSeed());
	this->criPRSubset = criPRSubset;
	
	readCRI(criPRSubset, cri);
	if(cri.isEmpty())
		cout << "CRI is empty!" << endl;
	else
		cout << "CRI is NOT empty!" << endl;
	cri.search(fid, criBlock);

	cout << "CRIBlock: Size = " << criBlock.getSize() << ", Seed = " << criBlock.getSeed() << endl;
	numBlocks = criBlock.getSize();
	numFileBlocks = numBlocks / BLOW_UP;
	
	cout << "No. of blocks " << numBlocks << " No. of file blocks " << numFileBlocks << endl;
	PRSubset filePRSubset(numBlocks, criBlock.getSeed());
	this->filePRSubset = filePRSubset;
	b_index_t blockIndices[numBlocks];
	filePRSubset.get(blockIndices, numBlocks);

	byte dataBlocksBytes[numBlocks*BLOCK_SIZE];
	readD(blockIndices, numBlocks, dataBlocksBytes);

//	printhex(dataBlocksBytes, numBlocks*BLOCK_SIZE, "BLOCK BYTES");
	size_t size = numFileBlocks*MAX_BLOCK_DATA_SIZE;
	int lastBlockSize = 0;
	byte contents[size];
	memset(contents, 0, size);
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
			if(block.getDataSize() < MAX_BLOCK_DATA_SIZE)
				lastBlockSize = block.getDataSize();
		}
	}
//	printchars(contents, lastBlockSize+(numFileBlocks-1)*MAX_BLOCK_DATA_SIZE, "FILE");

	byte file[(numFileBlocks-1)*MAX_BLOCK_DATA_SIZE + lastBlockSize];
	for(int i = 0; i < fileBlocks.size(); i++){
		byte block[BLOCK_SIZE];
		fileBlocks[i].getDecrypted(block);
		memcpy(&file[i*MAX_BLOCK_DATA_SIZE], block, fileBlocks[i].getDataSize());
	}
	printchars(file, (numFileBlocks-1)*MAX_BLOCK_DATA_SIZE + lastBlockSize, "FILE FROM VECTOR");
	cout << filename << " size is " << lastBlockSize + (numFileBlocks-1)*MAX_BLOCK_DATA_SIZE << " last block size is " << lastBlockSize << endl;

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

void OnlineSession::write(string filename, byte contents[], size_t size){
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

	byte decryptedBlocks[numBlocks*MAX_BLOCK_DATA_SIZE];
//	cout << "----------------------------------------CRI----------------------------------------" << endl;   
	for(int i = 0; i < numBlocks; i++){
		DataBlock block(blockIndices[i]);
		block.parse(&blocks[i*BLOCK_SIZE]);
		block.getDecrypted(&decryptedBlocks[i*MAX_BLOCK_DATA_SIZE]);
	}
	for(int i = 0; i < numBlocks; i++)
		printhex(&decryptedBlocks[i*BLOCK_SIZE], BLOCK_SIZE, "CRI BLOCKS");

	cri.parseBytes(decryptedBlocks, numBlocks*BLOCK_SIZE);
//	cout << "----------------------------------------CRI (ends)----------------------------------------" << endl;   
}
