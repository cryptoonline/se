//
//  TBlock.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__TBlock__
#define __BlindStorage__TBlock__

#include <stdint.h>
#include <iostream>
#include "parameters.h"
#include "AES.h"
#include "Key.h"

class TBlock {
private:
	uint32_t version;
    uint32_t index;
	uint32_t prSubsetSize;
    uint32_t prSubsetSeed;
    unsigned char* block; /// Format: seed(4 bytes) || size(4 bytes) || version(4 bytes))
	unsigned char* encryptedBlock;
    static char* key;
	char* iv;
	static bool wasKeyGenerated;
		
	void make();
    void parse();

	unsigned char* ENC();
	unsigned char* DEC();
	void makeIV();
	void setupKey();	
    
public:
    TBlock();
	~TBlock();
    TBlock(unsigned char* encryptedBlock, uint32_t index);
    void set(uint32_t prSubsetSize, uint32_t prSubsetSeed, uint32_t index);
    uint32_t getPrSubsetSize();
    uint32_t getPrSubsetSeed();

	void update(uint32_t prSubsetSize, uint32_t prSubsetSeed);
    unsigned char* getEncrypted();
	unsigned char* getDecrypted();
};

#endif /* defined(__BlindStorage__TBlock__) */
