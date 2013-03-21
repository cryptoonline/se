//
//  D.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "D.h"

D::D(){
    blocks = new DataBlock*[TOTALBLOCKS];
    for(int i = 0; i < TOTALBLOCKS; i++){
        blocks[i] = new DataBlock();
    }
}

D::~D(){
    delete[] blocks;
}

void D::add(unsigned char* rawData, uint32_t blockIndex){
    
}

unsigned char* D::get(uint32_t blockIndex){
    
}

void D::remove(uint32_t blockIndex){
    
}

void D::encryptUnusedBlocks(){
    
    for(int i = 0; i < TOTALBLOCKS; i++)
        if(!blocks[i]->isOccupied())
            blocks[i]->enc
    
}
