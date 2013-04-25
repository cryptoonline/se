//
//  TBlock.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "TBlock.h"

TBlock::TBlock(){
    prSubsetSeed = 0;
    prSubsetSize = 0;
}

TBlock::TBlock(unsigned char* block){
    this->block = block;
    parse();
}

void TBlock::set(uint32_t prSubsetSize, uint32_t prSubsetSeed){
    this->prSubsetSize = prSubsetSize;
    this->prSubsetSeed = prSubsetSeed;
    block = new unsigned char[sizeof(this->prSubsetSize) + sizeof(this->prSubsetSeed)];
    make();
}

unsigned char* TBlock::get(){
    return block;
}

uint32_t TBlock::getPrSubsetSeed(){
    return prSubsetSeed;
}

uint32_t TBlock::getPrSubsetSize(){
    return prSubsetSize;
}

void TBlock::make(){
    memcpy(block, static_cast<unsigned char*>(static_cast<void*>(&prSubsetSeed)), sizeof(prSubsetSeed));
    memcpy(block, static_cast<unsigned char*>(static_cast<void*>(&prSubsetSize)), sizeof(prSubsetSize));
}

void TBlock::parse(){
    prSubsetSeed = *(uint32_t*)block;
    prSubsetSize = *(uint32_t*)block+(uint32_t)sizeof(prSubsetSeed);
}