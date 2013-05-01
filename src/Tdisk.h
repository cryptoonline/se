//
//  Tdisk.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__Tdisk__
#define __BlindStorage__Tdisk__

#include <stdint.h>
#include <iostream>
#include "TBlock.h"
#include "parameters.h"
#include "fileID.h"
#include "PRSubset.h"
#include "FFX.h"
#include <ext/hash_map>
#include <vector>
#include "Ddisk.h"

using namespace std;

class Tdisk{
private:
    struct CRI{
        uint32_t prSubsetSeed;
        uint32_t prSubsetSize;
        unsigned char fid[32];
    };
    
    TBlock** T;
    __gnu_cxx::hash_map<uint32_t, vector<struct CRI> > map; //Higher 32 bits of value is storing seed and lower 32 bits are storing size
    uint32_t crHash(uint32_t input);
    uint32_t PRP(uint32_t input);

public:
    Tdisk();
    ~Tdisk();
    void addFile(fileID fid, PRSubset &prSubset);
    void finalize(Ddisk &D);
    void upload();
	void writeToDisk();
    
};

#endif /* defined(__BlindStorage__Tdisk__) */
