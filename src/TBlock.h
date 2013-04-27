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

class TBlock {
private:
    uint32_t prSubsetSize;
    uint32_t prSubsetSeed;
    unsigned char* block; /// Format: seed(4 bytes) || size(4 bytes)
    void make();
    void parse();
    
public:
    TBlock();
    TBlock(unsigned char* block);
    void set(uint32_t prSubsetSize, uint32_t prSubsetSeed);
    uint32_t getPrSubsetSize();
    uint32_t getPrSubsetSeed();
    unsigned char* get();
};

#endif /* defined(__BlindStorage__TBlock__) */
