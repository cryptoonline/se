//
//  D.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__D__
#define __BlindStorage__D__

#include <iostream>
#include "parameters.h"
#include "DataBlock.h"


class D {
private:
    DataBlock** blocks;
    
public:
    D();
    ~D();
    void add(unsigned char* rawData, uint32_t blockIndex);
    unsigned char* get(uint32_t blockIndex);
    void remove(uint32_t blockIndex);
    void encryptUnusedBlocks();


};

#endif /* defined(__BlindStorage__D__) */
