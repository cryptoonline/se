//
//  FFX.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__FFX__
#define __BlindStorage__FFX__

#include <iostream>

class FFX {
private:

    
public:
    unsigned char* encrypt(unsigned char* bytes);
    unsigned char* keyGen();
};

#endif /* defined(__BlindStorage__FFX__) */
