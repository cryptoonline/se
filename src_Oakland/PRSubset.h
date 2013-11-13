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
using std::endl;
using std::cerr;

#include <fcntl.h>

#include "parameters.h"
#include <fstream>

#include "rdtsc.h"
#include <stdio.h>

#include "boost/generator_iterator.hpp"
#include "boost/random.hpp"


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
	
	void get(b_index_t subset[], b_index_t size);
};

#endif /* defined(__BlindStorage__PRSubset__) */