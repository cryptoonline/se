//
//  OnlineSession.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "OnlineSession.h"

DataStructures OnlineSession::dataStructures;

OnlineSession::OnlineSession(Communicator &communicator){
    this->communicator = communicator;
	this->communicator.connect();
//	loadDataStructures();
//    this->criPrSubset = criPrSubset;
//    this->criFid = criFid;
//    this->filename = filename;
//    this->fileCompleteID = fileFid.get();
}

OnlineSession::~OnlineSession(){
}

bool OnlineSession::get(string filename, int32_t size, vector<unsigned char>& fileContents){
	fileID fid(filename);
	this->fid = fid;
	fileCompleteID = fid.get();
	int32_t numBlocks = (int32_t)ceil((double)size/MAX_BLOCK_DATA_SIZE) * BLOW_UP;
    readTRecord();
    bool fileExists = tblock->getPrSubsetSize();
	
//	bool readFile 		= (size == 0) &&  fileExists;
//	bool fileNotFound 	= (size == 0) && !fileExists;
//	bool updateFile 	= (size != 0) &&  fileExists;
//	bool writeFile		= (size != 0) && !fileExists;

	if(!fileExists && size == 0){
		cout << "BlindStorage: File not found. " << __FILE__ << ":" << __LINE__ <<  endl;
		exit(0);
	}
	if(size > 0)
		filePrSubset = new PRSubset(numBlocks);
		
//	if(!fileExists)
//		criPrSubset = new PRSubset(4);
	readCRI();
    getFile(fileContents);
	return fileExists;
}

void OnlineSession::update(unsigned char* input, uint32_t size, string filename){
    uint32_t numBlocks = (uint32_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE) * BLOW_UP;
	//cout << "FILE SIZE IN UPDATE IS " << size << endl;
	//cout << "BLOCK IN UPDATE ARE " << numBlocks << endl;
	//cout << "filePrSubset size is " << filePrSubset->getSize() << endl;
	
	//cout << "BLOCKs in PRSUBSET ARE " << filePrSubset->getSize() << endl;
	uint32_t* prSubsetArray = filePrSubset->get();
	for(int i = 0; i < numBlocks; i++)
		D_session[i] = new DataBlock(prSubsetArray[i]);

	printdec(filePrSubset->get(), size*BLOW_UP/MAX_BLOCK_DATA_SIZE, "PRUSBET in UPDATE");
	
	writeFile(input, size, fid, *filePrSubset);
	writeFinalize(numBlocks);
	
	encryptedfileBlockstoBeWritten = new unsigned char*[numBlocks];
    for(int i = 0; i < filePrSubset->getSize(); i++)
        encryptedfileBlockstoBeWritten[i] = D_session[i]->getEncrypted();

	//cout << "D Updated" << endl;
		PRSubset* criPrSubset = cri.getCRIPrSubset();
		struct CRI criEntry;
		criEntry.prSubsetSeed = filePrSubset->getSeed();
		criEntry.prSubsetSize = filePrSubset->getSize();
		memcpy(criEntry.fid, fid.get(), 32);
		//cout << "CRI Subset size is " << criPrSubset->getSize() << " and seed is " << criPrSubset->getSeed() << endl;
		tblock->update(criPrSubset->getSize(), criPrSubset->getSeed());
		//printhex(tblock->getEncrypted(), T_BLOCK_SIZE, "TBLOCK ENCRYPTED IN ONLINESESSION");
		//cout << "FID IS " << fid.getPRPofHigherID() << " cri is " << criPrSubset->getSize() << " seed is " << criPrSubset->getSeed() << endl;
//		memcpy(&T[fid.getPRPofHigherID()], tblock->getEncrypted(), T_BLOCK_SIZE);
    writeT(fid.getPRPofHigherID(), tblock->getEncrypted());
    writeD(filePrSubset->get(), filePrSubset->getSize(), encryptedfileBlockstoBeWritten);
}

void OnlineSession::remove(){
//    get();
//    for(int i = 0; filePrSubset->getSize() / BLOW_UP; i++){
//        fileID zeroFid;
//        extractedFileBlocks[i]->update(zeroFid, NULL, 0);
//    }
//    Update other blocks also, just have to update their version and encrypt
    
//    writeT();
//    writeD(prSubset.get(),
    
}

void OnlineSession::rename(){
    
}

void OnlineSession::readTRecord(){
    uint32_t TRecordIndex = fid.getPRPofHigherID();
	//cout << "Reading " << TRecordIndex << endl;
	//cout << "Index in " << __FUNCTION__ << " is " << TRecordIndex << endl;
	unsigned char* block = readT(TRecordIndex);
    tblock = new TBlock(block, TRecordIndex);
	//printhex(tblock->getEncrypted(), T_BLOCK_SIZE, "READ T RECORD ENCRYPTED DATA");
	//printhex(tblock->getDecrypted(), T_BLOCK_SIZE, "READ T RECORD DECRYPTED DATA");
}

bool OnlineSession::readCRI(){
	cri.addTBlock(*tblock);
	uint32_t* blocksLocations = cri.getBlocksLocations();
	printdec(blocksLocations, 4, "GET CRI");
	cri.addFile(readD(blocksLocations, cri.getNumBlocks()), fid);
    int32_t match = cri.searchFID(fileCompleteID);
    if(match != -1){
		unsigned char* criEntry = cri.getEntry(match);
		//cout << "Seed for file is " << *(uint32_t*)(criEntry) << " and size is " << *(uint32_t*)(criEntry + 4) << endl;
        filePrSubset = new PRSubset(*(uint32_t*)(criEntry+4), *(uint32_t*)(criEntry));
        return true;
    }
    return false; //return false if file not found.
}

void OnlineSession::getFile(vector<unsigned char>& fileContents){
    uint32_t* blockLocations = filePrSubset->get();
    int32_t prSubsetSize = filePrSubset->getSize();
	//cout << "PRSUBSET BLOCKS WHEN GETTING FILE IS " << prSubsetSize << endl;
    unsigned char** encryptedBlocksData = readD(blockLocations, prSubsetSize);
    decryptedFileBlocksRead = new unsigned char*[prSubsetSize];
    D_session = new DataBlock*[prSubsetSize];
    extractedFileBlocks = new DataBlock*[prSubsetSize/BLOW_UP];
    int j = 0;
    for(int i = 0; i < prSubsetSize; ++i){
        D_session[i] = new DataBlock(blockLocations[i], encryptedBlocksData[i]);
		decryptedFileBlocksRead[i] = D_session[i]->getDecrypted();
        if(D_session[i]->checkFileID(fid)){
			extractedFileBlocks[j] = D_session[i];
			//printhex(decryptedFileBlocksRead[i], BLOCK_SIZE, "GET FILE");
			fileContents.insert(fileContents.end(), &decryptedFileBlocksRead[i][0], &decryptedFileBlocksRead[i][MAX_BLOCK_DATA_SIZE+1]);
            j++;
      }
    }
	
	for(int k = fileContents.size(); k > 0; k--)
		if(fileContents[k] == 1){
			fileContents.resize(k-1);
			break;
		}
	print_docid_t(fileContents, "FILE CONTENTS IN ONLINE SESSION GET");
}

unsigned char* OnlineSession::readT(uint32_t TRecordIndex){
	unsigned char* TEntry = new unsigned char[T_BLOCK_SIZE];
//	communicator.tGet(TRecordIndex, reinterpret_cast<char*>(TEntry));
	dataStructures.tGet(TRecordIndex, TEntry);
	return TEntry;
}

void OnlineSession::writeT(uint32_t TRecordIndex, unsigned char* block){
//	communicator.tPut(TRecordIndex, reinterpret_cast<char*>(block));
	dataStructures.tPut(TRecordIndex, block);
}

unsigned char** OnlineSession::readD(uint32_t* blockLocations, uint32_t numBlocks){
	unsigned char** blocks = new unsigned char*[numBlocks];
	for(int i = 0; i < numBlocks; i++)
		blocks[i] = new unsigned char[BLOCK_SIZE];
	dataStructures.dGet(blockLocations, numBlocks, blocks);
	//for(int i = 0; i < numBlocks; i++){
	//	communicator.dGet(blockLocations[i], reinterpret_cast<char*>(blocks[i]));
	//}
	//communicator.dGet(blockLocations, blocks, numBlocks);
//	cout << "Inefficient readD for memory, will make it efficinet once done with debuggin" << endl;
//	for( int i = 0; i < numBlocks; i++){
//		memcpy(blocks[i], &D[blockLocations[i]*BLOCK_SIZE], BLOCK_SIZE); 
		//printhex(blocks[i], BLOCK_SIZE, "BLOCK in readD");
//	}		
	return blocks;
}

void OnlineSession::writeD(uint32_t* blockLocations, uint32_t numBlocks, unsigned char** blocks){
	dataStructures.dPut(blockLocations, numBlocks, blocks);	
//	for(int i = 0; i < numBlocks; i++){
//		communicator.dPut(blockLocations[i], reinterpret_cast<char*>(blocks[i]));
//		memcpy(&D[blockLocations[i]*BLOCK_SIZE], blocks[i], BLOCK_SIZE);
//	}
}
           
           
int32_t OnlineSession::search(unsigned char** arrayToSearchIn, unsigned char* arrayToSearchFor, int32_t rows, int32_t cols, int32_t startCol, int32_t startRow){
    int matches;
	//printhex(arrayToSearchIn, rows, cols, "arrayToSearchIn");
    for(int i = startRow; i < rows; i++){
        matches = 0;
		//printhex(arrayToSearchFor, cols, "arrayToSearchFor");
        for(int j = 0; j < cols; j++){
			if( arrayToSearchFor[j] == arrayToSearchIn[i][j+startCol]){
                matches++;
				if(matches == 32)
					return i;
			}
			else
				continue;
        }
    }
    return -1;
}

void OnlineSession::loadDataStructures(){

	string TF = T_FILE;
	string DF = D_FILE;

	int64_t TSize = readFileSize(TF);
	int64_t DSize = readFileSize(DF);
	
	T = new char[TSize];
	D = new char[DSize];
	
	cout << "Reading T from memory: " << endl;
	loadFile(TF, T, TSize);
	cout << "Reading T completed." << endl;

	cout << "Reading D from memory: " << endl;
	loadFile(DF, D, DSize);
	cout << "Reading D completed." << endl;
}

void OnlineSession::loadFile(string filename, char* input, int64_t size){
	std::ifstream file(filename.c_str());
	file.seekg(0, std::ios::beg);
	file.read(input, size);
	file.close();
}

int64_t OnlineSession::readFileSize(string filename){
	std::ifstream file(filename.c_str(), std::ios_base::binary | std::ios_base::ate);
	return file.tellg();
}

void OnlineSession::writeFile(unsigned char* fileBytes, uint32_t fileSize, fileID& fid, PRSubset& prSubset){
	vector<uint32_t> emptyBlocks = getEmptyBlocks(prSubset);
	//cout << "BLOCKS IN WRITE FILE ARE " << prSubset.getSize() << endl;
	if(emptyBlocks.size()*BLOCK_SIZE < fileSize){
		cerr << "Not enough empty blocks" << endl;
		exit(1);
	}
	makeBlocks(fileBytes, emptyBlocks.data(), fid, fileSize);
}

void OnlineSession::makeBlocks(unsigned char* fileBytes, uint32_t* prSubset, fileID &fid, uint32_t filesize){
	int counter = 0;
	int32_t numBlocks = (int32_t)ceil((double)filesize/(double)MAX_BLOCK_DATA_SIZE);
	//cout << "NUMBER OF BLOCKS " << numBlocks << endl;
	unsigned char* block = new unsigned char[numBlocks * BLOCK_SIZE]();
	for(; counter < numBlocks - 1; counter++){
		//cout << "COUNTER " << counter << endl;
		memcpy(&block[counter*BLOCK_SIZE], &fileBytes[counter*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
//		D_session[counter] = new DataBlock(prSubset[counter], fid, &blocks[counter*BLOCK_SIZE], MAX_BLOCK_DATA_SIZE);
//		D_session[counter] = new DataBlock(prSubset[counter]);
		D_session[counter]->update(fid, &block[counter*BLOCK_SIZE], MAX_BLOCK_DATA_SIZE);
	}
	int32_t sizeOfLastBlock = (int32_t)filesize - ((int32_t)filesize/MAX_BLOCK_DATA_SIZE) * MAX_BLOCK_DATA_SIZE;
	//printhex(&block[counter*BLOCK_SIZE], BLOCK_SIZE, "EMPTY BLOCK");
	//printhex(&fileBytes[counter*MAX_BLOCK_DATA_SIZE], 145, "FILE DATA");
	memcpy(&block[counter*BLOCK_SIZE], &fileBytes[counter*MAX_BLOCK_DATA_SIZE], 140);
//	D_session[counter] = new DataBlock(prSubset[counter]);
	D_session[counter]->update(fid, &block[counter*BLOCK_SIZE], MAX_BLOCK_DATA_SIZE);
}

void OnlineSession::writeFinalize(uint32_t numBlocks){
	for(uint32_t i = 0; i < numBlocks; i++){
		//cout << "BLOCK in ONLINE SESSION finalize " << i << endl;
		if(!D_session[i]->isOccupied()){
			D_session[i]->encryptIfEmpty();
		}
	}
}

vector<uint32_t> OnlineSession::getEmptyBlocks(PRSubset &prSubset){
	uint32_t prSubsetSize = prSubset.getSize();
	//cout << "SIZE OF THE SUBSET IS " << prSubsetSize << endl;
	uint32_t* subset = prSubset.get();
	std::vector<uint32_t> emptyBlocks;
	for(int i = 0; i < prSubsetSize; i++){
		if(!D_session[i]->isOccupied())
			emptyBlocks.push_back(subset[i]);
	}
	return emptyBlocks;
}
//bool OnlineSession::searchCRI(){
//    unsigned char* fileID = fileFid.get();
//    int matches = 0;
//    for(int k = 0; k < 32; k++){
//        for(int i = 0; i < criPrSubset.getSize(); i++){
//            for(int j = 0; j < MAX_BLOCK_DATA_SIZE; j++){
//                if(decryptedCriFile[i][j] == fileID[k]){
//                    k++;
//                    matches++;
//                    if(matches == 32)
//                        return true;
//                }
//                else
//                    matches = 0;
//            }
//        }
//    }
//    return false;
//}
