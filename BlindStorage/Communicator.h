//
//  Communicator.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/1/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__Communicator__
#define __BlindStorage__Communicator__

#include <iostream>
#include "DataBlock.h"

class Communicator{
    
public:
    void upload(uint32_t* blockLocations, unsigned char* data, string filename);
    void download(uint32_t* blockLocations, unsigned char* data, string filename);
};

#endif /* defined(__BlindStorage__Communicator__) */
