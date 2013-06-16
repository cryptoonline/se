//
//  PRSubset.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "PRSubset.h"

PRSubset::PRSubset(){
	size = 0;
	seed = 0;
}

PRSubset::PRSubset(prSubsetSize_t size){
	this->size = size;
	generateSeed();
}

PRSubset::PRSubset(prSubsetSize_t size, prSubsetSeed_t seed){
	this->size = size;
	this->seed = seed;
}

void PRSubset::make(b_index_t subset[]){
	srand(seed);
	for(int i = 0; i < size; i++){
		subset[i] = rand() % TOTAL_BLOCKS;
	}
}

void PRSubset::get(b_index_t subset[]){
	make(subset); 
}

prSubsetSize_t PRSubset::getSize() const{
	return size;
}

prSubsetSeed_t PRSubset::getSeed() const{
	return seed;
}

void PRSubset::generateSeed(){
	srand(clock());
	this->seed = rand();
	
//	uint32_t seed[1];
//	if(1)
//		rdrand_32(seed, 0))
//	else
//		std::cerr << "RDRAND failed. It can be solved by changing the second parameter of rdrand_64 function to the number of required retries.";
}



