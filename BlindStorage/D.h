////
////  D.h
////  BlindStorage
////
////  Created by Muhammad Naveed on 3/21/13.
////  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
////
//
//#ifndef __BlindStorage__D__
//#define __BlindStorage__D__
//
//#include <iostream>
//#include "parameters.h"
//#include "DataBlock.h"
//
//
//class D {
//private:
//    DataBlock** blocks; /*!< Array of DataBlocks i.e. D */
//    
//public:
//    /*! A constructor */
//    D();
//    
//    void addFile(unsigned char* fileData, uint64_t PRSubset[]
//
//    /*! A destructor */
//    ~D();
//    
//    /*!
//     This function can be used to add data blocks to the D data structure.
//     */
//    void add(uint32_t blockIndex, uint32_t fid, unsigned char* rawData, uint32_t size);
//    
//    /*!uu
//     This function can be used to get the contents of the block, it will return block with the trailer i.e. padding byte, file id and version.
//     */
//    unsigned char* get(uint32_t blockIndex);
//        
//    /*!
//     This function can be used to remove block from the D data structure.
//     */
//    void remove(uint32_t blockIndex);
//    
//    /*!
//     This function can be used to remove a group of blocks from the D data structure in same time as required to remove single block.
//     */
//    void remove(uint32_t blocksToRemove[], uint32_t size);
//    
//    /*!
//     This function can be used to encrypt the blocks that are not used. This function is made available to avoid encrypting all the datablocks which will mean encrypting the used datablocks twice.
//     */
//    void encryptUnusedBlocks();
//
//
//};
//
//#endif /* defined(__BlindStorage__D__) */
