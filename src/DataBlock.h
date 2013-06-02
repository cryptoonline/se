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

#include <vector>
using std::vector;

#include <fstream>

#include "AES.h"
#include "parameters.h"
#include "fileID.h"
#include "Key.h"
#include "Debug.h"

class DataBlock{
public:
	DataBlock();
	~DataBlock();

	void setKey(byte key[]);

	void loadRawDataBlock(b_index_t index, fileID fid, istream& in, version_t version = 0);
	
	void loadDataBlock(b_index_t index, istream& in);
	void update(fileID fid, byte block[], uint32_t dataSize);

	bool comparefileID(fileID& fid);
	bool isCRIBlock();

private:
	b_index_t index;
	version_t version;
	fileID fid;
	bool isPadded;
	bool isCRI;

	bool isBlockEncrypted;

	byte block[];

	static byte key[AES_KEY_SIZE];
	byte iv[AES_BLOCK_SIZE];

	void encrypt();
	void decrypt();

	void addPadding();
	void removePadding();

	void getEncrypted();
	void getDecrypted();
};

#endif /* defined(__BlindStorage__DataBlock__) */
