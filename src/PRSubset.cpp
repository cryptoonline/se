//
//  PRSubset.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "PRSubset.h"

PRSubset::PRSubset(){
}

PRSubset::PRSubset(uint32_t size){
    this->size = size;
    subset = new uint32_t[size];
    generateSeed();
    make();
}

PRSubset::PRSubset(uint32_t size, uint32_t seed){
    this->size = size;
    subset = new uint32_t[size];
    this->seed = seed;
    make();
}

void PRSubset::make(){
    srand(seed);
    for(int i = 0; i < size; i++){
        subset[i] = rand() % TOTAL_BLOCKS;
	}
}

uint32_t PRSubset::getSeed(){
    return seed;
}

uint32_t PRSubset::getSize(){
    return size;
}

uint32_t* PRSubset::get(){ //modify it to uint32_t in other places
    return subset;
}

void PRSubset::generateSeed(){
    //uint32_t seed[1];
    //if(1)//rdrand_32(seed, 0))
    this->seed = rand();
	
    //else
    //    std::cerr << "RDRAND failed. It can be solved by changing the second parameter of rdrand_64 function to the number of required retries.";
}



