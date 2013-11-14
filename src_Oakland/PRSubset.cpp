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

	byte keyBytes[HMAC_KEY_SIZE];
	memset(keyBytes, 0, HMAC_KEY_SIZE);
	hash.setKey(keyBytes);
//	Key key(PRSUBSET_KEY, HMAC_KEY_SIZE);
//	key.get(keyBytes);
}

//PRSubset::PRSubset(prSubsetSize_t size){
//	if(size < PRSUBSET_SIZE_LOWER_BOUND)
//		size = PRSUBSET_SIZE_LOWER_BOUND;
//	this->size = size;
//	generateSeed();
//}

PRSubset::PRSubset(prSubsetSize_t size, prSubsetSeed_t seed){
	if(size < PRSUBSET_SIZE_LOWER_BOUND)
		size = PRSUBSET_SIZE_LOWER_BOUND;
	this->size = size;
	this->seed = seed;
}

PRSubset::PRSubset(prSubsetSize_t size, string filename){
	
	byte keyBytes[HMAC_KEY_SIZE];
	memset(keyBytes, 0, HMAC_KEY_SIZE);
	hash.setKey(keyBytes);

	if(size < PRSUBSET_SIZE_LOWER_BOUND)
		size = PRSUBSET_SIZE_LOWER_BOUND;

	this->size = size;
	generateSeed(filename);
}

void PRSubset::make(b_index_t subset[]){
//	srand(seed);
//	for(int i = 0; i < size; i++){
//		subset[i] = rand() % TOTAL_BLOCKS;
//	}

	typedef boost::mt19937 RNGType;
	RNGType rng(seed);
	boost::uniform_int<> urandDevice(0, TOTAL_BLOCKS-1);
	boost::variate_generator< RNGType, boost::uniform_int<> >
		urand(rng, urandDevice);

	for(prSubsetSize_t i = 0; i < size; i++)
		subset[i] = urand();
}

void PRSubset::get(b_index_t subset[], b_index_t size){
	if(size < PRSUBSET_SIZE_LOWER_BOUND){
		std::cerr << "PRSubset size is lower than the lower bound. Exiting!" << endl;
		exit(1);
	}
	make(subset); 
}

prSubsetSize_t PRSubset::getSize() const{
	return size;
}

prSubsetSeed_t PRSubset::getSeed() const{
	return seed;
}

void PRSubset::generateSeed(string filename){
	byte mac[SHA_BLOCK_SIZE];
	hash.doFinal(filename, mac);
	seed = *(prSubsetSeed_t*)(mac);
}
/*
void PRSubset::generateSeed(){
//	srand(clock());
//	srand(time(NULL));
//	this->seed = rand();

//	srand(rdtsc());
//	seed = rand();

//	int randomData = open("/dev/random", O_RDONLY); //To get large randomness
	int randomData = open("/dev/urandom", O_RDONLY);
	read(randomData, &seed, sizeof(prSubsetSeed_t));
	close(randomData);
	
//	uint32_t seed[1];
//	if(1)
//		rdrand_32(seed, 0))
//	else
//		std::cerr << "RDRAND failed. It can be solved by changing the second parameter of rdrand_64 function to the number of required retries.";
}
*/
