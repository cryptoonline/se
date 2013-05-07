//
//  OnlineSession.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "OnlineSession.h"

OnlineSession::OnlineSession(Communicator &communicator){
    this->communicator = communicator;
	this->communicator.connect();
	loadDataStructures();
//    this->criPrSubset = criPrSubset;
//    this->criFid = criFid;
//    this->filename = filename;
//    this->fileCompleteID = fileFid.get();
}

OnlineSession::~OnlineSession(){
}

void OnlineSession::get(string filename, int32_t size, vector<unsigned char>& fileContents){
	fileID fid(filename);
	this->fid = fid;
	fileCompleteID = fid.get();
	int32_t numBlocks = (int32_t)ceil((double)size/MAX_BLOCK_DATA_SIZE) * BLOW_UP;
    getCRI();
    bool fileExists = parseCRI();
	
	bool readFile 		= (size == 0) &&  fileExists;
	bool fileNotFound 	= (size == 0) && !fileExists;
	bool updateFile 	= (size != 0) &&  fileExists;
	bool writeFile		= (size != 0) && !fileExists;

	if(fileNotFound){
		cout << "BlindStorage: File not found. " << __FILE__ << ":" << __LINE__ <<  endl;
		exit(1);
	} else if(writeFile || updateFile)
		filePrSubset = new PRSubset(numBlocks);
		
    getFile(fileContents);
}

void OnlineSession::update(unsigned char* input, uint32_t size, string filename){
    uint32_t numBlocks = (uint32_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE) * BLOW_UP;
	cout << "FILE SIZE IN UPDATE IS " << size << endl;
	cout << "BLOCK IN UPDATE ARE " << numBlocks << endl;
	cout << "filePrSubset size is " << filePrSubset->getSize() << endl;
	getchar();
	
	cout << "BLOCKs in PRSUBSET ARE " << filePrSubset->getSize() << endl;
	uint32_t* prSubsetArray = filePrSubset->get();
	for(int i = 0; i < numBlocks; i++)
		D_session[i] = new DataBlock(prSubsetArray[i]);

	printdec(filePrSubset->get(), size*BLOW_UP/MAX_BLOCK_DATA_SIZE, "PRUSBET in UPDATE");
	
	writeFile(input, size, fid, *filePrSubset);
	writeFinalize(numBlocks);
	
	encryptedfileBlockstoBeWritten = new unsigned char*[numBlocks];
    for(int i = 0; i < filePrSubset->getSize(); i++)
        encryptedfileBlockstoBeWritten[i] = D_session[i]->getEncrypted();

	cout << "D Updated" << endl;
    if(tblock != NULL)
		tblock->update(filePrSubset->getSize(), filePrSubset->getSeed());
	else
		cerr << "Access NULL pointer  at line " << __LINE__ << " in " << __PRETTY_FUNCTION__; 
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

void OnlineSession::getCRI(){
    uint32_t TRecordIndex = fid.getPRPofHigherID();
	cout << "Index in " << __FUNCTION__ << " is " << TRecordIndex << endl;
    //communicator.connect();
	unsigned char* block = readT(TRecordIndex);
    tblock = new TBlock(block, TRecordIndex);
	cri.addTBlock(*tblock);
	uint32_t* blocksLocations = cri.getBlocksLocations();
	printdec(blocksLocations, 4, "GET CRI");
	cri.addFile(readD(blocksLocations, cri.getNumBlocks()));
	
//	cout << __FUNCTION__ << " Size is " << tblock->getPrSubsetSize() << " Seed is " << tblock->getPrSubsetSeed() << endl;
//    criPrSubset = new PRSubset(tblock->getPrSubsetSize(), tblock->getPrSubsetSeed());
//    uint32_t* blockLocations = criPrSubset->get();
//    
//    int32_t criNumBlocks = criPrSubset->getSize();
//    unsigned char** criFile = readD(blockLocations, criNumBlocks);
//	printhex(criFile, criNumBlocks, BLOCK_SIZE, "CRI FILE BLOCKS");    
//
//    criFileBlocks = new DataBlock*[criNumBlocks];
//	decryptedCriFile = new unsigned char*[criNumBlocks];
//    for(int i = 0; i < criNumBlocks; i++){
//		cout << "Processing cri blocks " << i << "/" << criNumBlocks - 1 << endl;
//		//printhex(criFile[i], BLOCK_SIZE, "Processing following block");
//        criFileBlocks[i] = new DataBlock(blockLocations[i], criFile[i]);
//        decryptedCriFile[i] = criFileBlocks[i]->getDecrypted();
//	//	if(criFileBlocks[i]->checkFileID(fid)){
//	//		goodCriBlocks[i] = decryptedCriFile[i]; 
//	//		printhex(goodCriBlocks[i], BLOCK_SIZE, "Good Block");
//	//	}
//		//printhex(decryptedCriFile[i], BLOCK_SIZE, "Decrypted Block");
//    }
}

bool OnlineSession::parseCRI(){
	
//	//printhex(decryptedCriFile[1], BLOCK_SIZE, "Decrypted Block in parse CRI");
//    int criFileBytes = criPrSubset->getSize() * MAX_BLOCK_DATA_SIZE;
//	cout << "CRI have " << criPrSubset->getSize() << "blocks and " << criFileBytes << " bytes." << endl;
//    criEntries = new unsigned char*[criFileBytes / CRI_ENTRY_SIZE]; //make 40 a parameter in parameters.h
//	//unsigned char** decrytedCriDataBlocks = new unsigned char*[criPrSubset->getSize()];
//	//for( int i = 0; i < criPrSubset->getSize(); i++)
//	//	decryptedCriDataBlocks[i] = new unsigned char[BLOCK_SIZE];
//
//    for(int i = 0; i < criFileBytes / CRI_ENTRY_SIZE; i++){
//        criEntries[i] = new unsigned char[CRI_ENTRY_SIZE]();
//        memcpy(criEntries[i], decryptedCriFile[0]+i*CRI_ENTRY_SIZE, CRI_ENTRY_SIZE);
//       // if((i*CRI_ENTRY_SIZE) / MAX_BLOCK_DATA_SIZE >= 0)
//         //   i += BLOCK_SIZE - MAX_BLOCK_DATA_SIZE;
//		//printhex(criEntries[i], CRI_ENTRY_SIZE, "CRI Entry");
//	//	cout << endl;
//    }
////make it use checkFileID
    int32_t match = cri.searchFID(fileCompleteID);
    if(match != -1){
		unsigned char* criEntry = cri.getEntry(match);
		cout << "Seed for file is " << *(uint32_t*)(criEntry) << " and size is " << *(uint32_t*)(criEntry + 4) << endl;
		getchar();
        filePrSubset = new PRSubset(*(uint32_t*)(criEntry+4), *(uint32_t*)(criEntry));
        return true;
    }
    
    return false; //return false if file not found.
}
    

//uint32_t OnlineSession::getFileSize(){
//    return filePrSubset->getSize();
//}
void OnlineSession::getFile(vector<unsigned char>& fileContents){
    uint32_t* blockLocations = filePrSubset->get();
    int32_t prSubsetSize = filePrSubset->getSize();
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
			fileContents.insert(fileContents.end(), &decryptedFileBlocksRead[i][0], &decryptedFileBlocksRead[i][MAX_BLOCK_DATA_SIZE]);
            j++;
      }
    }
}

unsigned char* OnlineSession::readT(uint32_t TRecordIndex){
	unsigned char* TEntry = new unsigned char[T_BLOCK_SIZE];
//	communicator.tGet(TRecordIndex, reinterpret_cast<char*>(TEntry));
	memcpy(TEntry, &T[TRecordIndex*T_BLOCK_SIZE], 12);
	return reinterpret_cast<unsigned char*>(TEntry);
}

void OnlineSession::writeT(uint32_t TRecordIndex, unsigned char* block){
//	communicator.tPut(TRecordIndex, reinterpret_cast<char*>(block));
	memcpy(&T[TRecordIndex*T_BLOCK_SIZE], block, T_BLOCK_SIZE);
}

unsigned char** OnlineSession::readD(uint32_t* blockLocations, uint32_t numBlocks){
	unsigned char** blocks = new unsigned char*[numBlocks];
	for(int i = 0; i < numBlocks; i++)
		blocks[i] = new unsigned char[BLOCK_SIZE];
	//for(int i = 0; i < numBlocks; i++){
	//	communicator.dGet(blockLocations[i], reinterpret_cast<char*>(blocks[i]));
	//}
	//communicator.dGet(blockLocations, blocks, numBlocks);
	cout << "Inefficient readD for memory, will make it efficinet once done with debuggin" << endl;
	for( int i = 0; i < numBlocks; i++){
		memcpy(blocks[i], &D[blockLocations[i]*BLOCK_SIZE], BLOCK_SIZE); 
		//printhex(blocks[i], BLOCK_SIZE, "BLOCK in readD");
	}		
	return blocks;
}

void OnlineSession::writeD(uint32_t* blockLocations, uint32_t numBlocks, unsigned char** blocks){
	for(int i = 0; i < numBlocks; i++){
//		communicator.dPut(blockLocations[i], reinterpret_cast<char*>(blocks[i]));
		memcpy(&D[blockLocations[i]*BLOCK_SIZE], blocks[i], BLOCK_SIZE);
	}
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
	cout << "BLOCKS IN WRITE FILE ARE " << prSubset.getSize() << endl;
	if(emptyBlocks.size()*MAX_BLOCK_DATA_SIZE < fileSize){
		cerr << "Not enough empty blocks" << endl;
		exit(1);
	}
	makeBlocks(fileBytes, emptyBlocks.data(), fid, fileSize);
}

void OnlineSession::makeBlocks(unsigned char* fileBytes, uint32_t* prSubset, fileID &fid, size_t filesize){
	int counter = 0;
	int32_t numBlocks = (int32_t)ceil((double)filesize/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP;
	unsigned char* block = new unsigned char[numBlocks * BLOCK_SIZE]();
	for(; counter < numBlocks - 1; counter++){
		memcpy(&block[counter*BLOCK_SIZE], &fileBytes[counter*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE);
//		D_session[counter] = new DataBlock(prSubset[counter], fid, &blocks[counter*BLOCK_SIZE], MAX_BLOCK_DATA_SIZE);
//		D_session[counter] = new DataBlock(prSubset[counter]);
		D_session[counter]->update(fid, &block[counter*BLOCK_SIZE], MAX_BLOCK_DATA_SIZE);
	}
	uint32_t sizeOfLastBlock = (uint32_t)filesize - ((uint32_t)filesize/MAX_BLOCK_DATA_SIZE) * MAX_BLOCK_DATA_SIZE;
	memcpy(&blocks[counter*BLOCK_SIZE], &fileBytes[counter*MAX_BLOCK_DATA_SIZE], sizeOfLastBlock);
//	D_session[counter] = new DataBlock(prSubset[counter]);
	D_session[counter]->update(fid, &block[counter*BLOCK_SIZE], MAX_BLOCK_DATA_SIZE);
}

void OnlineSession::writeFinalize(uint32_t numBlocks){
	for(uint32_t i = 0; i < numBlocks; i++){
		cout << "BLOCK in ONLINE SESSION finalize " << i << endl;
		if(!D_session[i]->isOccupied()){
			D_session[i]->encryptIfEmpty();
		}
	}
}

vector<uint32_t> OnlineSession::getEmptyBlocks(PRSubset &prSubset){
	uint32_t prSubsetSize = prSubset.getSize();
	cout << "SIZE OF THE SUBSET IS " << prSubsetSize << endl;
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
