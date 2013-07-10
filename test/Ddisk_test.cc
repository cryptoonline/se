//
// Ddisk_test.cc
// BlindStorage Test
//

#include "gtest/gtest.h"

#include "./../src/Ddisk.h"
#include "./../src/DataBlock.h"
#include "./../src/parameters.h"
#include "./../src/Debug.h"

TEST(DdiskTest, Test1){
	srand(clock());

	Ddisk D;
	size_t numOfBytes = MAX_BLOCK_DATA_SIZE*5;
//	size_t numByteswithTrailer = numOfBytes+BLOCK_SIZE-MAX_BLOCK_DATA_SIZE;
	byte bytes[numOfBytes];
//	memset(bytes, 0, numByteswithTrailer);

	b_index_t numBlocks = (b_index_t)ceil((double)numOfBytes/(double)MAX_BLOCK_DATA_SIZE);
	cout << "BLOCKS " << numBlocks << endl;
	for(int i = 0; i < numBlocks; i++){
		memset(&bytes[i*MAX_BLOCK_DATA_SIZE], rand(), MAX_BLOCK_DATA_SIZE);
	}

	fileID fid("DdiskTest");
	PRSubset prSubset(BLOW_UP*numBlocks, 10);
	b_index_t subset[prSubset.getSize()];
	
	prSubset.get(subset, prSubset.getSize());

	D.addFile(bytes, numOfBytes, fid, prSubset);
	D.encryptEmptyBlocks();
	
	byte blockData[BLOCK_SIZE];


	srand(clock());
	int random = rand();
	b_index_t index = random % numBlocks;
	DataBlock block(index);
	cout << "Index " << index << " Random is " << random << " numBlocks " << numBlocks << endl;
	D.getBlock(subset[index], block);
	block.getDecrypted(blockData);
	printhex(blockData, BLOCK_SIZE, "Actual");
	printhex(&bytes[index*MAX_BLOCK_DATA_SIZE], BLOCK_SIZE, "Expected");
	ASSERT_TRUE(0 == std::memcmp(blockData, &bytes[index*MAX_BLOCK_DATA_SIZE], MAX_BLOCK_DATA_SIZE));
}
