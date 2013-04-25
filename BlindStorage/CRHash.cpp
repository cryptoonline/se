//
//  CRHash.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/18/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "CRHash.h"

unsigned char* CRHash::doFinal(unsigned char* input){
    
}

uint32_t CRHash::doFinal(uint32_t input){
    CRHash crHash;
    unsigned char* hashBytes = crHash.doFinal(static_cast<unsigned char*>(static_cast<void*>(&input)));
    uint32_t hash = *(uint32_t *)hashBytes;
    delete hashBytes;
    return hash;
}