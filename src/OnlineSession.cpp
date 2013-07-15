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

int OnlineSession::retrieveTBlock(){
	byte tBlockBytes[TBLOCK_SIZE];
	readT(fid.getHigherID(), tBlockBytes);
	TBlock tBlock(fid.getHigherID());

	tBlock.parse(tBlockBytes);
	printhex(tBlockBytes, TBLOCK_SIZE, __PRETTY_FUNCTION__);
	cout << "Higher FID" << __PRETTY_FUNCTION__ << " " << fid.getHigherID() << endl;
	cout << "TBlock: Size = " << tBlock.getSize() << ", Seed = " << tBlock.getSeed() << endl;
	this->tBlock = tBlock;
	if(tBlock.getSize() == 0)
		return 0;
	return 1;
}

size_t OnlineSession::retrieveDBlocks(b_index_t numBlocksToWrite){
	size_t filesize = 0;
	
	numBlocks = max(numBlocksToWrite, criBlock.getSize());

	numFileBlocks = numBlocks / BLOW_UP;
	
	cout << "No. of blocks " << numBlocks << " No. of file blocks " << numFileBlocks << endl;
	PRSubset filePRSubset(numBlocks, criBlock.getSeed());
	cout << "File PRSubset size is " << filePRSubset.getSize() << endl;
	this->filePRSubset = filePRSubset;
	b_index_t blockIndices[filePRSubset.getSize()];
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	printdec(blockIndices, filePRSubset.getSize(), "FILE INDICES");
	byte dataBlocksBytes[numBlocks*BLOCK_SIZE];
	readD(blockIndices, numBlocks, dataBlocksBytes);

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

	return filesize;
}

void extractFileBlocks(){
	
}

int OnlineSession::retrieveCRIBlock(){
	PRSubset criPRSubset(tBlock.getSize(), tBlock.getSeed());
	this->criPRSubset = criPRSubset;
	
	readCRI(criPRSubset, cri);
	if(cri.isEmpty()){
		std::cerr << "CRI shouldn't be empty at this point:" << __FILE__ << ":" << __LINE__ << endl;
		exit(1);
	}

	criBlockIndex = cri.search(fid, criBlock);
	cout << "CRIBlock: Size = " << criBlock.getSize() << ", Seed = " << criBlock.getSeed() << endl;
	if(criBlockIndex == -1)
		return 0;
	return 1;
}

size_t OnlineSession::read(string filename, byte*& file, b_index_t numBlocksToRead){
	cout << "Number of blocks to read " << numBlocksToRead << endl;
	cout << "Reading " << filename << endl;

	this->filename = filename;
	fileID fid(filename);
	this->fid = fid;
	
	retrieveTBlock();
	retrieveCRIBlock();
	
	size_t filesize = retrieveDBlocks();
	
	file = new byte[filesize];
	for(int i = 0; i < fileBlocks.size(); i++){
		byte block[BLOCK_SIZE];
		fileBlocks[i].getDecrypted(block);
		memcpy(&file[i*MAX_BLOCK_DATA_SIZE], block, fileBlocks[i].getDataSize());
	}

	return filesize;
}

void OnlineSession::update(string filename, byte contents[], size_t size){
	b_index_t numBlocksToWrite = (b_index_t)ceil(((double)size/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
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
		criBlock.make(filePRSubset.getSize(), filePRSubset.getSeed(), lowerFid);	
		cri.addFile(filePRSubset.getSize(), filePRSubset.getSeed(), lowerFid);
		
		TBlock tBlock(fid.getHigherID());
		tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
		this->tBlock = tBlock;
	
		cout << "TBlock size is " << tBlock.getSize() << " and seed is " << tBlock.getSeed() << endl;
		retrieveDBlocks(numBlocksToWrite);
	}
	else if(retrieveCRIBlock() == 0){
		PRSubset filePRSubset(numBlocksToWrite);
		this->filePRSubset = filePRSubset;
		cout << "Size is " << cri.size() << endl;
		criBlock.make(filePRSubset.getSize(), filePRSubset.getSeed(), lowerFid);
		cri.addFile(filePRSubset.getSize(), filePRSubset.getSeed(), lowerFid);
		b_index_t criNumBlocks = ((int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
		cout << "CRIBlock: Size=" << criPRSubset.getSize() << " Seed=" << criPRSubset.getSeed() << endl;
		cout << "criNumBlocks=" << criNumBlocks << "criPRSubset.getSize()=" << criPRSubset.getSize() << endl;
		if(criNumBlocks > criPRSubset.getSize())	
			tBlock.update(criNumBlocks, criPRSubset.getSeed());
		else
			tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
		retrieveDBlocks(numBlocksToWrite);
	}
	else{
		retrieveDBlocks(numBlocksToWrite);
		if(numBlocksToWrite > filePRSubset.getSize())
			cri.updateFile(numBlocksToWrite, filePRSubset.getSeed(), criBlockIndex);
		tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
	}
	cout << "TBLOCK: Size=" << tBlock.getSize() << ", Seed=" << tBlock.getSeed() << endl;
	//TODO: This can be optimized by making the blockIndices Array class member
	b_index_t blockIndices[filePRSubset.getSize()];
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	cout << "Update blocks " << updatedFileBlocksIndices.size() << " Num blocks to write " << numBlocksToWrite << endl;
	byte blocksBytes[numBlocksToWrite*BLOCK_SIZE];

	cout << "Number blocks to write " << numBlocksToWrite << " Updated fileblock size is " << updatedFileBlocksIndices.size() << endl;
	cout << "Total data blocks read are " << blocks.size() << endl;
	if(numBlocksToWrite > updatedFileBlocksIndices.size()){
		for(int i = 0; i < blocks.size(); i++){
			byte b[BLOCK_SIZE] = {0};
			blocks[i].getDecrypted(b);
			printhex(b, BLOCK_SIZE, "EMPTY BLOCK");
			cout << "Is block empty " << blocks[i].isOccupied() << endl;
			if(!(blocks[i].isOccupied()) && (updatedFileBlocksIndices.size() <= numBlocksToWrite))
				updatedFileBlocksIndices.push_back(i);
			else
				blocks[i].updateVersion();
		}
	}

	if(updatedFileBlocksIndices.size() < numBlocksToWrite){
		cerr << "Update unsuccessful: Not enought blocks available." << endl;
		exit(1);
	}
	cout << "Update blocks " << updatedFileBlocksIndices.size() << endl;

	byte blocksBytesDecrypted[numBlocksToWrite*BLOCK_SIZE];
	printchars(contents, size, "UPDATED FILE");
	int i = 0;
	cout << "Number of blocks to be written are " << numBlocksToWrite << endl;
	for(; i < numBlocksToWrite/BLOW_UP - 1; ++i){
		cout << "Updating block " << i << " with Index " << updatedFileBlocksIndices[i] << endl;
//		blocks[updatedFileBlocksIndices[i]].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
		printhex(&contents[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE, "INPUT OUTSIDE");
		blocks[updatedFileBlocksIndices[i]].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
		blocks[updatedFileBlocksIndices[i]].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);
//		printhex(&blocksBytes[i*BLOCK_SIZE], BLOCK_SIZE, "ENCRYPTED UPDATED BLOCK");
		blocks[updatedFileBlocksIndices[i]].getDecrypted(&blocksBytesDecrypted[i*MAX_BLOCK_DATA_SIZE]);
	}
	cout << "Updating last block " << i << " with Index " << updatedFileBlocksIndices[i] << endl;
	dataSize_t sizeOfLastBlock = (dataSize_t)(size - (size/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE); 
//	blocks[updatedFileBlocksIndices[i]].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
	printhex(&contents[i*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock, "INPUT OUTSIDE");
	blocks[updatedFileBlocksIndices[i]].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
	blocks[updatedFileBlocksIndices[i]].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);
	printhex(&blocksBytes[i*BLOCK_SIZE], sizeOfLastBlock, "ENCRYPTED UPDATED LAST BLOCK");
	blocks[updatedFileBlocksIndices[i]].getDecrypted(&blocksBytesDecrypted[i*MAX_BLOCK_DATA_SIZE]);

	printchars(blocksBytesDecrypted, size, "UPDATED FILE");
//	for(int i = 0; i < numBlocksToWrite; i++)
//		printhex(&blocksBytes[i*BLOCK_SIZE], BLOCK_SIZE, "BLOCKS");

	printdec(blockIndices, filePRSubset.getSize(), "FILE INDICES UPDATED");
//	dcomm.printD("ORIGINAL BLOCKS");
	writeD(blockIndices, filePRSubset.getSize(), blocksBytes);
//	dcomm.printD("MODIFIED BLOCKS");
	byte tBlockBytes[TBLOCK_SIZE];
	byte tBlockBytesDecrypted[TBLOCK_SIZE];
	tBlock.getDecrypted(tBlockBytesDecrypted);
	printhex(tBlockBytesDecrypted, TBLOCK_SIZE, "TBLOCK UPDATE");
	tBlock.getEncrypted(tBlockBytes);
	printhex(tBlockBytes, TBLOCK_SIZE, "ENCRYPTED TBLOCK");

//	byte tBlockBytesDecrypted[TBLOCK_SIZE];
//	tBlock.getDecrypted(tBlockBytesDecrypted);
//	printhex(tBlockBytesDecrypted, TBLOCK_SIZE, "TBLOCK DECRYPTED");
	writeT(fid.getHigherID(), tBlockBytes);

	writeCRI();	

	dcomm.writeToDisk();
}

void OnlineSession::write(string filename, byte contents[], size_t size){
	byte* previous_file;
	cout << "Size of the file is " << size << endl;
	b_index_t numBlocksToWrite = (b_index_t)ceil(((double)size/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
	size_t previous_size = read(filename, previous_file, numBlocksToWrite);

	cout << "Previous file size is " << size << " No. of blocks to write " << numBlocksToWrite << endl;
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
		else{
			cri.updateFile(filePRSubset.getSize(), filePRSubset.getSeed(), criBlockIndex);
			byte criBytes[cri.size()];
			cri.makeBytes(criBytes);

			for(int i = 0; i < cri.size()/CRI_BLOCK_SIZE; i++)
				printhex(&criBytes[i*CRI_BLOCK_SIZE], CRI_BLOCK_SIZE, __PRETTY_FUNCTION__);
			cout << "File size is " << filePRSubset.getSize() << endl;
		}
		tBlock.update(criPRSubset.getSize(), criPRSubset.getSeed());
	}

	//TODO: This can be optimized by making the blockIndices Array class member
	b_index_t blockIndices[filePRSubset.getSize()];
	filePRSubset.get(blockIndices, filePRSubset.getSize());

	cout << "Update blocks " << updatedFileBlocksIndices.size() << " Num blocks to write " << numBlocksToWrite << endl;
	byte blocksBytes[numBlocksToWrite*BLOCK_SIZE];

	cout << "Number blocks to write " << numBlocksToWrite << " Updated fileblock size is " << updatedFileBlocksIndices.size() << endl;
	cout << "Total data blocks read are " << blocks.size() << endl;
	if(numBlocksToWrite > updatedFileBlocksIndices.size()){
		for(int i = 0; i < blocks.size(); i++){
			byte b[BLOCK_SIZE] = {0};
			blocks[i].getDecrypted(b);
			printhex(b, BLOCK_SIZE, "EMPTY BLOCK");
			cout << "Is block empty " << blocks[i].isOccupied() << endl;
			if(!(blocks[i].isOccupied()) && (updatedFileBlocksIndices.size() <= numBlocksToWrite))
				updatedFileBlocksIndices.push_back(i);
			else
				blocks[i].updateVersion();
		}
	}

	cout << "Update blocks " << updatedFileBlocksIndices.size() << endl;

	byte blocksBytesDecrypted[numBlocksToWrite*BLOCK_SIZE];
	printchars(contents, size, "UPDATED FILE");
	int i = 0;
	cout << "Number of blocks to be written are " << numBlocksToWrite << endl;
	for(; i < numBlocksToWrite/BLOW_UP - 1; ++i){
		cout << "Updating block " << i << " with Index " << updatedFileBlocksIndices[i] << endl;
//		blocks[updatedFileBlocksIndices[i]].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
		printhex(&contents[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE, "INPUT OUTSIDE");
		blocks[updatedFileBlocksIndices[i]].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
		blocks[updatedFileBlocksIndices[i]].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);
//		printhex(&blocksBytes[i*BLOCK_SIZE], BLOCK_SIZE, "ENCRYPTED UPDATED BLOCK");
		blocks[updatedFileBlocksIndices[i]].getDecrypted(&blocksBytesDecrypted[i*MAX_BLOCK_DATA_SIZE]);
	}
	cout << "Updating last block " << i << " with Index " << updatedFileBlocksIndices[i] << endl;
	dataSize_t sizeOfLastBlock = (dataSize_t)(size - (size/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE); 
//	blocks[updatedFileBlocksIndices[i]].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
	printhex(&contents[i*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock, "INPUT OUTSIDE");
	blocks[updatedFileBlocksIndices[i]].update(fid, &contents[i*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
	blocks[updatedFileBlocksIndices[i]].getEncrypted(&blocksBytes[i*BLOCK_SIZE]);
	printhex(&blocksBytes[i*BLOCK_SIZE], sizeOfLastBlock, "ENCRYPTED UPDATED LAST BLOCK");
	blocks[updatedFileBlocksIndices[i]].getDecrypted(&blocksBytesDecrypted[i*MAX_BLOCK_DATA_SIZE]);

	printchars(blocksBytesDecrypted, size, "UPDATED FILE");
//	for(int i = 0; i < numBlocksToWrite; i++)
//		printhex(&blocksBytes[i*BLOCK_SIZE], BLOCK_SIZE, "BLOCKS");

	printdec(blockIndices, filePRSubset.getSize(), "FILE INDICES UPDATED");
//	dcomm.printD("ORIGINAL BLOCKS");
	writeD(blockIndices, filePRSubset.getSize(), blocksBytes);
//	dcomm.printD("MODIFIED BLOCKS");
	byte tBlockBytes[TBLOCK_SIZE];
	
	tBlock.getEncrypted(tBlockBytes);

//	byte tBlockBytesDecrypted[TBLOCK_SIZE];
//	tBlock.getDecrypted(tBlockBytesDecrypted);
//	printhex(tBlockBytesDecrypted, TBLOCK_SIZE, "TBLOCK DECRYPTED");
	writeT(fid.getHigherID(), tBlockBytes);

	writeCRI();	

	dcomm.writeToDisk();
//	byte* file;
//	string filename2 = "/Users/naveed/BStore/datasets/testdir/1.";
//	size_t filesize = read(filename2, file);
//	printchars(file, filesize, "FILE READ in Write");
//	delete[] file;

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
//	for(int i = 0; i < numBlocks; i++)
//		printhex(&decryptedBlocks[i*BLOCK_SIZE], BLOCK_SIZE, "CRI BLOCKS");

	cri.parseBytes(decryptedBlocks, numBlocks*BLOCK_SIZE);
}

void OnlineSession::writeCRI(){
	byte criBytes[cri.size()];
	cri.makeBytes(criBytes);

	for(int i = 0; i < cri.size()/CRI_BLOCK_SIZE; i++){
		printhex(&criBytes[i*CRI_BLOCK_SIZE], CRI_BLOCK_SIZE, __PRETTY_FUNCTION__);
	}
	b_index_t criBlocksIndices[criPRSubset.getSize()];
	criPRSubset.get(criBlocksIndices, criPRSubset.getSize());

	byte encryptedBlocks[criBlocks.size()*BLOCK_SIZE];

	int i = 0;
	for(; i < criPRSubset.getSize() - 1; i++){
		cout << "Updating CRI block " << i << endl;
		criBlocks[i].update(criFid, &criBytes[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
		criBlocks[i].getEncrypted(&encryptedBlocks[i*BLOCK_SIZE]);
	}
	cout << "Updating last CRI block " << i << endl;
	dataSize_t sizeOfLastBlock = (dataSize_t)ceil(cri.size()-(cri.size()/MAX_BLOCK_DATA_SIZE)*MAX_BLOCK_DATA_SIZE);
	criBlocks[i].update(criFid, &criBytes[i*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
	criBlocks[i].getEncrypted(&encryptedBlocks[i*BLOCK_SIZE]);

	writeD(criBlocksIndices, criPRSubset.getSize(), encryptedBlocks);
}
