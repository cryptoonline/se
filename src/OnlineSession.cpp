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

bool OnlineSession::read(string filename, byte* contents){
	fileID fid(filename);
	this->fid = fid;

	byte tBlockBytes[TBLOCK_SIZE];
	readT(fid.getHigherID(), tBlockBytes);
	TBlock tBlock(fid.getHigherID());
	tBlock.parse(tBlockBytes);
	this->tBlock = tBlock;
	if(tBlock.getSize() == 0)
		return false;

	PRSubset criPRSubset(tBlock.getSize(), tBlock.getSeed());
	this->criPRSubset = criPRSubset;
	parseCRI(criPRSubset, criBlock);

	numBlocks = criBlock.getSize();
	numFileBlocks = numBlocks / BLOW_UP;

	PRSubset filePRSubset(numBlocks, criBlock.getSeed());
	this->filePRSubset = filePRSubset;
	b_index_t blockIndices[numBlocks];

	byte dataBlocksBytes[numBlocks][BLOCK_SIZE];
	readD(blockIndices, numBlocks, dataBlocksBytes);
	
	for(int i = 0; i < numBlocks; i++){
		DataBlock block;
		block.parse(dataBlocksBytes[i]);
		blocks.push_back(block);
		if(block.fidMatchCheck(fid))
			fileBlocks.push_back(block);
	}

	uint32_t lastBlockSize = fileBlocks[numFileBlocks].getDataSize();
	size_t size = (numFileBlocks-1)*MAX_BLOCK_DATA_SIZE + lastBlockSize;
	contents = new byte[size];

	for(int i = 0; i < numFileBlocks; i++)
		fileBlocks[i].getDecryptedData(&contents[i*MAX_BLOCK_DATA_SIZE]);
}

void OnlineSession::write(string filename, byte contents[], size_t size){
}

void OnlineSession::readT(t_index_t TRecordIndex, byte block[]){
}

void OnlineSession::writeT(t_index_t TRecordIndex, byte block[]){
}

void OnlineSession::readD(b_index_t blockIndices[], b_index_t numBlocks, byte[][BLOCK_SIZE]){
}

void OnlineSession::writeD(b_index_t blockIndices[], b_index_t numBlocks, byte[][BLOCK_SIZE]){
}

void OnlineSession::parseCRI(PRSubset& prSubset, CRIBlock& block){
}
