//
//  CRHash.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/18/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__CRHash__
#define __BlindStorage__CRHash__

#include <iostream>

class CRHash {
private:
    
public:
    unsigned char* doFinal(unsigned char* input);
    uint32_t doFinal(uint32_t input);
};

#endif /* defined(__BlindStorage__CRHash__) */
