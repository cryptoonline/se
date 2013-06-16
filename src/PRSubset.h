//
//  PRSubset.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__PRSubset__
#define __BlindStorage__PRSubset__

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include "parameters.h"
#include <fstream>

class PRSubset {
private:
	prSubsetSize_t size;
	prSubsetSeed_t seed;
	
	prSubsetSeed_t getRDRAND();
	void generateSeed();
	
	void make(b_index_t subset[]);
    
public:
	PRSubset();
	PRSubset(prSubsetSize_t size);
	PRSubset(prSubsetSize_t size, prSubsetSeed_t seed);
	
	prSubsetSize_t getSize() const;
	prSubsetSeed_t getSeed() const;
	
	void get(b_index_t subset[]);
};

#endif /* defined(__BlindStorage__PRSubset__) */
