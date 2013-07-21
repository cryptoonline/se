//
// CRIBlock_test.cc
// BlindStorage Test
//

#include "gtest/gtest.h"

#include "./../src/CRIBlock.h"
#include "./../src/parameters.h"
#include "./../src/Debug.h"

TEST(CRIBlockTest, make){
	CRIBlock block;
	
	byte lowerFid[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
										 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
										 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 
										 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B};

	srand(clock());
	prSubsetSize_t size = rand();
	prSubsetSeed_t seed = rand();
	size_t filesize = rand();

	block.make(size, seed, filesize, lowerFid);

	byte blockExpected[CRI_BLOCK_SIZE];
	memcpy(blockExpected, static_cast<byte*>(static_cast<void*>(&size)), sizeof(prSubsetSize_t));
	memcpy(blockExpected+sizeof(prSubsetSize_t), static_cast<byte*>(static_cast<void*>(&seed)), sizeof(prSubsetSeed_t));
	memcpy(blockExpected+sizeof(prSubsetSize_t)+sizeof(prSubsetSeed_t), static_cast<byte*>(static_cast<void*>(&filesize)), sizeof(size_t));
	memcpy(blockExpected+sizeof(prSubsetSize_t)+sizeof(prSubsetSeed_t)+sizeof(size_t), lowerFid, LOWERFID_SIZE);

	byte blockActual[CRI_BLOCK_SIZE];
	block.get(blockActual);
	EXPECT_TRUE(0 == std::memcmp(blockActual, blockExpected, CRI_BLOCK_SIZE));
}

TEST(CRIBlockTest, parseSize){
	CRIBlock block;
	prSubsetSize_t sizeExpected = 117464123;
	byte blockData[] = {0x3b, 0x5c, 0x00, 0x07, 0x14, 0x29, 0xa7, 0x28, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b}; 

	block.parse(blockData);

	prSubsetSize_t sizeActual = block.getSize();
	
	EXPECT_EQ(sizeExpected, sizeActual);
}

TEST(CRIBlockTest, parseSeed){
	CRIBlock block;
	prSubsetSeed_t seedExpected = 682043668;
	byte blockData[] = {0x3b, 0x5c, 0x00, 0x07, 0x14, 0x29, 0xa7, 0x28, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b}; 

	block.parse(blockData);

	prSubsetSeed_t seedActual = block.getSeed();
	
	EXPECT_EQ(seedExpected, seedActual);
}

TEST(CRIBlockTest, match){
	CRIBlock block;
	prSubsetSize_t sizeExpected = 117464123;
	byte blockData[] = {0x3b, 0x5c, 0x00, 0x07, 0x14, 0x29, 0xa7, 0x28, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b}; 

	block.parse(blockData);

	prSubsetSize_t sizeActual = block.getSize();

	byte lowerFid[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
										 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
										 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 
										 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B};

	EXPECT_TRUE(block.match(lowerFid));
}
