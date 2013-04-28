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

class Communicator{
    
public:
    void send(unsigned char* data);
	void send(unsigned char** data);
    unsigned char* receive();
	unsigned char** receive(bool getIn2D);
};

#endif /* defined(__BlindStorage__Communicator__) */
