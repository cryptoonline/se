//
//  parameters.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__parameters__
#define __BlindStorage__parameters__

#include <stdint.h>
#include <iostream>

const uint32_t TOTAL_BLOCKS = 1048576;//1048576; ///2^20
const uint32_t BLOCK_SIZE = 2048;//-1-2*sizeof(uint32_t); /*! 1+2*sizeof(uint32_t) is subtracted, so the block size after adding trailer is equal to power of 2 */
const uint32_t MAX_BLOCK_DATA_SIZE = BLOCK_SIZE - 1 - sizeof(uint32_t) - 32; /*!< This repesents the size (bytes) of the data part of block, maximum size of the block can be 2^32 as limited by the method used for creating unique IV for every block. */

const uint16_t MAX_T_SIZE = 2^16 - 1;

const uint16_t MAX_NO_OF_COLLISIONS = 1024;

const uint16_t BLOW_UP = 4;

const int16_t CRI_ENTRY_SIZE = 40;
#endif /* defined(__BlindStorage__parameters__) */
