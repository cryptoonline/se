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

class PRF {
private:
	static unsigned char* key;
    
public:
    uint32_t encrypt(unsigned char* plaintext, uint16_t size, uint32_t  bitMask);
    unsigned char* keyGen();
};

#endif /* defined(__BlindStorage__PRF__) */
