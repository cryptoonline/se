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

unsigned char* OnlineSession::get(string filename, PRSubset* prSubset){
	fileID fid(filename);
	this->fid = fid;
	fileCompleteID = fid.get();
	//printhex(fileCompleteID, 32, "GET");
    getCRI();
    bool fileExists = parseCRI();
	if(!fileExists){
		filePrSubset = prSubset;
		cout << "File not found, this can happen during write." << endl;
	}
    getFile();
	printchars(fileDataRead, 3*MAX_BLOCK_DATA_SIZE, "WHOLE FILE");
    return fileDataRead;
}

void OnlineSession::update(unsigned char* input, uint32_t size, string filename){
//    uint32_t numBlocks = (uint32_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);
    
	if(size > filePrSubset->getSize()){
		int seed = filePrSubset->getSeed();
		delete filePrSubset;	
		filePrSubset = new PRSubset(size, seed);
	}
    Ddisk D(filePrSubset->getSize());
    D.addFile(input, size, fid, *filePrSubset);
   	D.finalize();
    
	fileBlockstoBeWritten = D.get();
    for(int i = 0; i < filePrSubset->getSize(); i++)
        encryptedfileBlockstoBeWritten[i] = fileBlockstoBeWritten[i]->getEncrypted();
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

	cout << __FUNCTION__ << " Size is " << tblock->getPrSubsetSize() << " Seed is " << tblock->getPrSubsetSeed() << endl;
    criPrSubset = new PRSubset(tblock->getPrSubsetSize(), tblock->getPrSubsetSeed());
    uint32_t* blockLocations = criPrSubset->get();
    
    int32_t criNumBlocks = criPrSubset->getSize();
    unsigned char** criFile = readD(blockLocations, criNumBlocks);
	//printhex(criFile, criNumBlocks, BLOCK_SIZE, "CRI FILE BLOCKS");    

    criFileBlocks = new DataBlock*[criNumBlocks];
	decryptedCriFile = new unsigned char*[criNumBlocks];
    for(int i = 0; i < criNumBlocks; i++){
		cout << "Processing cri blocks " << i << "/" << criNumBlocks - 1 << endl;
		//printhex(criFile[i], BLOCK_SIZE, "Processing following block");
        criFileBlocks[i] = new DataBlock(blockLocations[i], criFile[i]);
        decryptedCriFile[i] = criFileBlocks[i]->getDecrypted();
	//	if(criFileBlocks[i]->checkFileID(fid)){
	//		goodCriBlocks[i] = decryptedCriFile[i]; 
	//		printhex(goodCriBlocks[i], BLOCK_SIZE, "Good Block");
	//	}
		//printhex(decryptedCriFile[i], BLOCK_SIZE, "Decrypted Block");
    }
}

bool OnlineSession::parseCRI(){
	//printhex(decryptedCriFile[0], BLOCK_SIZE, "Decrypted Block in parse CRI");
    int criFileBytes = criPrSubset->getSize() * MAX_BLOCK_DATA_SIZE;
	cout << "CRI have " << criPrSubset->getSize() << "blocks and " << criFileBytes << " bytes." << endl;
    criEntries = new unsigned char*[criFileBytes / CRI_ENTRY_SIZE]; //make 40 a parameter in parameters.h
	//unsigned char** decrytedCriDataBlocks = new unsigned char*[criPrSubset->getSize()];
	//for( int i = 0; i < criPrSubset->getSize(); i++)
	//	decryptedCriDataBlocks[i] = new unsigned char[BLOCK_SIZE];

    for(int i = 0; i < criFileBytes / CRI_ENTRY_SIZE; i++){
        criEntries[i] = new unsigned char[CRI_ENTRY_SIZE]();
        memcpy(criEntries[i], decryptedCriFile[0]+i*CRI_ENTRY_SIZE, CRI_ENTRY_SIZE);
       // if((i*CRI_ENTRY_SIZE) / MAX_BLOCK_DATA_SIZE >= 0)
         //   i += BLOCK_SIZE - MAX_BLOCK_DATA_SIZE;
		//printhex(criEntries[i], CRI_ENTRY_SIZE, "CRI Entry");
	//	cout << endl;
    }
//make it use checkFileID
    int32_t match = search(criEntries, fileCompleteID, criPrSubset->getSize(), CRI_ENTRY_SIZE, 8, 0);
    if(match != -1){
		cout << "Seed for file is " << *(uint32_t*)(criEntries[match]) << " and size is " << *(uint32_t*)(criEntries[match] + 4) << endl;
        filePrSubset = new PRSubset(*(uint32_t*)(criEntries[match]+4), *(uint32_t*)(criEntries[match]));
        return true;
    }
    
    return false; //return false if file not found.
}
    

//uint32_t OnlineSession::getFileSize(){
//    return filePrSubset->getSize();
//}
void OnlineSession::getFile(){
    uint32_t* blockLocations = filePrSubset->get();
    int32_t prSubsetSize = filePrSubset->getSize();
    unsigned char** encryptedBlocksData = readD(blockLocations, prSubsetSize);
    decryptedFileBlocksRead = new unsigned char*[prSubsetSize];
    fileDataRead = new unsigned char[prSubsetSize/BLOW_UP];

    fileBlocksRead = new DataBlock*[prSubsetSize];
    extractedFileBlocks = new DataBlock*[prSubsetSize/BLOW_UP];
    int j = 0;
    for(int i = 0; i < prSubsetSize; ++i){
        fileBlocksRead[i] = new DataBlock(blockLocations[i], encryptedBlocksData[i]);
        decryptedFileBlocksRead[i] = new unsigned char[BLOCK_SIZE]();
        memcpy(decryptedFileBlocksRead[i], fileBlocksRead[i]->getDecrypted(), BLOCK_SIZE);
		printchars(decryptedFileBlocksRead[i], BLOCK_SIZE, "DECRYPTED BLOCKS IN GET FILE");
        if(fileBlocksRead[i]->checkFileID(fid)){
			extractedFileBlocks[j] = fileBlocksRead[i];
            memcpy(&fileDataRead[0]+j*MAX_BLOCK_DATA_SIZE, decryptedFileBlocksRead[i], MAX_BLOCK_DATA_SIZE);
			cout << "Index" << j*MAX_BLOCK_DATA_SIZE << endl;
			printchars(&fileDataRead[j*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE, "EXTRACTED FILE DATA");
            j++;
			printchars(&fileDataRead[j*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE, "PREVIOUS EXTRACTED BLOCK");
      }
    }
	for(int i = 0; i < 3; i++)
		printchars(&fileDataRead[i*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE, "PARTS");
	printchars(fileDataRead, 3*MAX_BLOCK_DATA_SIZE, "WHOLE FILE IN GET FILE");
}

unsigned char* OnlineSession::readT(uint32_t TRecordIndex){
	unsigned char* TEntry = new unsigned char[T_BLOCK_SIZE];
//	communicator.tGet(TRecordIndex, reinterpret_cast<char*>(TEntry));
	memcpy(TEntry, &T[TRecordIndex*T_BLOCK_SIZE], 12);
	return reinterpret_cast<unsigned char*>(TEntry);
}

void OnlineSession::writeT(uint32_t TRecordIndex, unsigned char* block){
	communicator.tPut(TRecordIndex, reinterpret_cast<char*>(block));
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
		communicator.dPut(blockLocations[i], reinterpret_cast<char*>(blocks[i]));
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
