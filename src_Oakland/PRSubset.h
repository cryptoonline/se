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
#include "unordered_set"

#include "HashMAC.h"
#include "Key.h"

using std::unordered_set;

class PRSubset {
private:
	prSubsetSize_t size;
	prSubsetSeed_t seed;
	
	prSubsetSeed_t getRDRAND();
	void generateSeed(string filename);
	
	void make(b_index_t subset[]);

	HashMAC hash;
    
public:
	PRSubset();
//	PRSubset(prSubsetSize_t size);
	PRSubset(prSubsetSize_t size, prSubsetSeed_t seed);
	PRSubset(prSubsetSize_t size, string filename);	
	prSubsetSize_t getSize() const;
	prSubsetSeed_t getSeed() const;
	
	void get(b_index_t subset[], b_index_t size);
};

#endif /* defined(__BlindStorage__PRSubset__) */
