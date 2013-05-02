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

const uint32_t TOTAL_BLOCKS = 8048576;//1048576; ///2^20
const uint32_t BLOCK_SIZE = 512;//-1-2*sizeof(uint32_t); /*! 1+2*sizeof(uint32_t) is subtracted, so the block size after adding trailer is equal to power of 2 */
const uint32_t MAX_BLOCK_DATA_SIZE = BLOCK_SIZE - 1 - sizeof(uint32_t) - 32; /*!< This repesents the size (bytes) of the data part of block, maximum size of the block can be 2^32 as limited by the method used for creating unique IV for every block. */

const uint32_t MAX_T_SIZE = 16777216;
const uint16_t T_BLOCK_SIZE = 12;

const uint16_t MAX_NO_OF_COLLISIONS = 1024;

const uint16_t BLOW_UP = 4;

const int16_t CRI_ENTRY_SIZE = 40;

const int16_t KEY_SIZE = 16;

const int16_t T_RECORD_BITS = 24;
/****************************************FileNames****************************************/ 
//const char* D_file = "D";
//const char* T_file = "T";
const char D_KEYFILE[] = "/data/keys/D.KEY";
const char T_KEYFILE[] = "/data/keys/T.KEY";

#endif /* defined(__BlindStorage__parameters__) */
