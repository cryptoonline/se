//
//  DataBlock.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__DataBlock__
#define __BlindStorage__DataBlock__

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include "AES.h"
#include "parameters.h"
#include "fileID.h"

class DataBlock {
public:
	DataBlock();
	DataBlock(b_index_t index);
	~DataBlock();

	void setKey(byte key[]);

	void make(fileID fid, byte block[], int dataSize = MAX_BLOCK_DATA_SIZE, bool isCRI = false, version_t version = 0);
	
	void parse(byte block[]);
	void update(fileID fid, byte block[], int dataSize = MAX_BLOCK_DATA_SIZE, bool isCRI = false);
	void clear();

	bool comparefileID(fileID& fid);
	bool isCRIBlock();

	void getEncrypted(byte block[]);
	void getDecrypted(byte block[]);
	int getDataSize();

private:
	static b_index_t instanceCounter;
	b_index_t index;
	version_t version;
	fileID fid;
	bool isCRI;

	bool isBlockEncrypted;

	byte* block;

	int dataSize;

	static byte key[AES_KEY_SIZE];
	byte iv[AES_BLOCK_SIZE];

	void encrypt();
	void decrypt();
	void makeIV();

	void addTrailer();
	void addPadding();
	void removePadding();
};

#endif /* defined(__BlindStorage__DataBlock__) */
