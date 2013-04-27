//
//  PRSubset.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__PRSubset__
#define __BlindStorage__PRSubset__

#include <iostream>
#include "parameters.h"
//#include "/Users/naveed/crypto/BlindCloudStorage/paper/Code/BlindStorage/BlindStorage/lib/librdrand-1.1/rdrand.h"
//#include "/Users/naveed/crypto/BlindCloudStorage/paper/Code/BlindStorage/BlindStorage/lib/librdrand-1.1/config.h"

class PRSubset {
private:
    uint32_t* subset;
    uint32_t size;
    uint32_t seed;
    void make();
    uint32_t getRDRAND();
    void generateSeed();
    
public:
    PRSubset();
    PRSubset(uint32_t size);
    PRSubset(uint32_t size, uint32_t seed);
    uint32_t getSeed();
    uint32_t getSize();
    uint32_t* get();
};

#endif /* defined(__BlindStorage__PRSubset__) */
