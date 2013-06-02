//
//  PRF.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__PRF__
#define __BlindStorage__PRF__

#include <iostream>
#include <stdint.h>
#include "AES.h"
#include "parameters.h"

class PRF {
private:
	static byte key[];
	byte iv[AES_BLOCK_SIZE];
    
public:
    uint32_t encrypt(byte plaintext[], uint16_t size, uint32_t bitMask);
    void keyGen(byte key[]);
		void setKey(byte key[]);
		void setIV(byte iv[]);
};

#endif /* defined(__BlindStorage__PRF__) */
