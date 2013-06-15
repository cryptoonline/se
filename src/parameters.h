//
//  parameters.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include <stdint.h>

#ifndef __BlindStorage__parameters__
#define __BlindStorage__parameters__

/****************************************typedefs*****************************************/
typedef unsigned char byte;
typedef uint32_t b_index_t;
typedef uint32_t t_index_t;
typedef uint16_t version_t;
typedef uint32_t higherfid_t;
typedef uint32_t prSubsetSize_t;
typedef uint32_t prSubsetSeed_t;

#define TOTAL_BLOCKS 4194304
#define BLOCK_SIZE 512
const uint32_t MAX_BLOCK_DATA_SIZE = BLOCK_SIZE - 2 - sizeof(version_t) - 32;

#define TBLOCK_SIZE 10

/* BLOCK FORMAT */
/* DATA + Padding Byte + CRI Byte + FID + VERSION */
/* 			+ 1 					 + 1 		    + 32	+ 4				*/
#define PADBYTE_LOC MAX_BLOCK_DATA_SIZE
#define CRIBYTE_LOC MAX_BLOCK_DATA_SIZE + 1
#define FILEID_LOC MAX_BLOCK_DATA_SIZE + 2
#define VERSION_LOC MAX_BLOCK_DATA_SIZE + 2 + 32

#define FILEID_SIZE 32
#define HIGHERFID_SIZE 4
#define LOWERFID_SIZE 28

#define PRF_KEY_SIZE 16
#define PRF_BLOCK_SIZE 16
#define SHA_BLOCK_SIZE 32
#define HMAC_KEY_SIZE 20

#define CRI_BLOCK_SIZE 36

const uint32_t MAX_T_SIZE = 16384;//76;//16777216;
const uint16_t T_BLOCK_SIZE = 12;

const uint16_t MAX_NO_OF_COLLISIONS = 1024;

const uint16_t BLOW_UP = 4;

const int16_t KEY_SIZE = 16;

const int16_t T_RECORD_BITS = 24;

const uint32_t BIT_MASK = 0x00003FFF;

/****************************************AES parameters***********************************/
#define AES_BLOCK_SIZE 16
#define AES_KEY_SIZE 16

/****************************************FileNames****************************************/ 
#define D_KEYFILE "/Users/naveed/BStore/data/keys/D.KEY"
#define T_KEYFILE "/Users/naveed/BStore/data/keys/T.KEY"
#define D_FILE "/Users/naveed/BStore/data/D"
#define T_FILE "/Users/naveed/BStore/data/T" 

#endif /* defined(__BlindStorage__parameters__) */
