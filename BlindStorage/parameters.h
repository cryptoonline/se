//
//  parameters.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__parameters__
#define __BlindStorage__parameters__

#include <iostream>

const uint32_t TOTALBLOCKS = 1048576; ///2^20
const uint32_t BLOCKSIZE = 1024;//-1-2*sizeof(uint32_t); /*! 1+2*sizeof(uint32_t) is subtracted, so the block size after adding trailer is equal to power of 2 */

#endif /* defined(__BlindStorage__parameters__) */
