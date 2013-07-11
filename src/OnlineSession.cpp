//
//  OnlineSession.cpp
//  BlindStorage
//

#include "OnlineSession.h"

OnlineSession::OnlineSession(){
	numBlocks = 0;
	previousCriSize = 0;
	Key key(T_KEYFILE, AES_KEY_SIZE);
	byte keyBytes[AES_KEY_SIZE];
	key.get(keyBytes);
	tBlock.setKey(keyBytes);
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
	b_index_t blockIndices[filePRSubset.getSize()];
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	byte dataBlocksBytes[numBlocks*BLOCK_SIZE];
	readD(blockIndices, numBlocks, dataBlocksBytes);

	size_t filesize = 0;
	for(int i = 0; i < numBlocks; i++){
		DataBlock block(blockIndices[i]);
		block.parse(&dataBlocksBytes[i*BLOCK_SIZE]);
		blocks.push_back(block);
		if(block.fidMatchCheck(fid)){
			cout << "Block Matches!" << endl;
			fileBlocks.push_back(block);
			updatedFileBlocksIndices.push_back(i);
			if(block.getDataSize() < MAX_BLOCK_DATA_SIZE){
				filesize += block.getDataSize();
				break;
			} else {
				filesize += block.getDataSize();
			}
		}
	}

	file = new byte[filesize];
	for(int i = 0; i < fileBlocks.size(); i++){
		byte block[BLOCK_SIZE];
		fileBlocks[i].getDecrypted(block);
		memcpy(&file[i*MAX_BLOCK_DATA_SIZE], block, fileBlocks[i].getDataSize());
	}

	return filesize;
}

void OnlineSession::write(string filename, byte contents[], size_t size){
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
		tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
	}

	//TODO: This can be optimized by making the blockIndices Array class member
	b_index_t blockIndices[filePRSubset.getSize()];
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	byte blocksBytes[numBlocksToWrite*BLOCK_SIZE];

	if(numBlocksToWrite > updatedFileBlocksIndices.size()){
		for(int i = 0; i < blocks.size(); i++){
			if(!blocks[i].isOccupied() && updatedFileBlocksIndices.size() <= numBlocksToWrite)
				updatedFileBlocksIndices.push_back(i);
			else
				blocks[i].updateVersion();
		}
	}

	int i = 0;
	cout << "Number of blocks to be written are " << numBlocksToWrite << endl;
	for(; i < numBlocksToWrite - 1; ++i){
		cout << "Updating block " << i << " with Index " << updatedFileBlocksIndices[i] << endl;
		blocks[updatedFileBlocksIndices[i]].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
		blocks[updatedFileBlocksIndices[i]].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);
	}
	cout << "Updating last block " << i << " with Index " << updatedFileBlocksIndices[i] << endl;
	dataSize_t sizeOfLastBlock = (dataSize_t)(size - (size/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE); 
	blocks[updatedFileBlocksIndices[i]].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
	blocks[updatedFileBlocksIndices[i]].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);

	writeD(blockIndices, filePRSubset.getSize(), blocksBytes);

	byte tBlockBytes[TBLOCK_SIZE];
	
	tBlock.getEncrypted(tBlockBytes);

	byte tBlockBytesDecrypted[TBLOCK_SIZE];
	tBlock.getDecrypted(tBlockBytesDecrypted);
	printhex(tBlockBytesDecrypted, TBLOCK_SIZE, "TBLOCK DECRYPTED");
	writeT(fid.getHigherID(), tBlockBytes);

//	writeCRI();	

	delete[] previous_file;
}

void OnlineSession::readT(t_index_t TRecordIndex, byte block[]){
	dcomm.tGet(TRecordIndex, block);
	printhex(block, TBLOCK_SIZE, __PRETTY_FUNCTION__);
}

void OnlineSession::writeT(t_index_t TRecordIndex, byte block[]){
	dcomm.tPut(TRecordIndex, block);
}

void OnlineSession::readD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]){
	dcomm.dGet(blockIndices, numBlocks, blocks);
}

void OnlineSession::writeD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]){
	dcomm.dPut(blockIndices, numBlocks, blocks);
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
	fileID criFid(criFidBytes);
	this->criFid = criFid;

	for(int i = 0; i < numBlocks; i++){
		DataBlock block(blockIndices[i]);
		block.parse(&blocks[i*BLOCK_SIZE]);
		criBlocks.push_back(block);
		if(block.fidMatchCheck(criFid)){
			block.getDecrypted(&decryptedBlocks[i*MAX_BLOCK_DATA_SIZE]);
			fileCriBlocks.push_back(block);
			updatedCriFileBlocksIndices.push_back(i);
		}
	}
	for(int i = 0; i < numBlocks; i++)
		printhex(&decryptedBlocks[i*BLOCK_SIZE], BLOCK_SIZE, "CRI BLOCKS");

	cri.parseBytes(decryptedBlocks, numBlocks*BLOCK_SIZE);
}

void OnlineSession::writeCRI(){
	byte criBytes[cri.size()];
	cri.makeBytes(criBytes);
	
	b_index_t criBlocksIndices[criPRSubset.getSize()];
	criPRSubset.get(criBlocksIndices, criPRSubset.getSize());

	byte encryptedBlocks[criBlocks.size()];

	int i = 0;
	for(; i < 1; i++){
		cout << "Updating CRI block " << i << endl;
		criBlocks[i].update(criFid, &criBytes[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
		criBlocks[i].getEncrypted(&encryptedBlocks[i*BLOCK_SIZE]);
	}
	cout << "Updating last CRI block " << i << endl;
//	dataSize_t sizeOfLastBlock = (dataSize_t)ceil(cri.size()-(cri.size()/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE);
//	criBlocks[i].update(criFid, &criBytes[i*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
//	criBlocks[i].getEncrypted(&encryptedBlocks[i*BLOCK_SIZE]);

	writeD(criBlocksIndices, 1*BLOCK_SIZE, encryptedBlocks);
}
