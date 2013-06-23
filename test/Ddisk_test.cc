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
	size_t numByteswithTrailer = numOfBytes+BLOCK_SIZE-MAX_BLOCK_DATA_SIZE;
	byte bytes[numByteswithTrailer];
	memset(bytes, 0, numByteswithTrailer);

	b_index_t numBlocks = (b_index_t)ceil((double)numOfBytes/(double)MAX_BLOCK_DATA_SIZE);
	cout << "BLOCKS " << numBlocks << endl;
	for(int i = 0; i < numBlocks; i++){
		memset(&bytes[i*BLOCK_SIZE], rand(), MAX_BLOCK_DATA_SIZE);
	}

	fileID fid("DdiskTest");
	PRSubset prSubset(BLOW_UP*numBlocks, 10);
	b_index_t subset[BLOW_UP*numBlocks];
	
	if(BLOW_UP*numBlocks < PRSUBSET_SIZE_LOWER_BOUND)
		prSubset.get(subset, PRSUBSET_SIZE_LOWER_BOUND);
	else
		prSubset.get(subset, BLOW_UP*numBlocks);

	D.addBlocks(bytes, numOfBytes, fid, prSubset);
	D.encryptEmptyBlocks();
	
	byte blockData[BLOCK_SIZE];

	DataBlock block;

	srand(clock());
	int random = rand();
	b_index_t index = random % numBlocks;
	cout << "Index " << index << " Random is " << random << " numBlocks " << numBlocks << endl;
	D.getBlock(subset[index], block);
	block.getDecrypted(blockData);
	ASSERT_TRUE(0 == std::memcmp(blockData, &bytes[index*BLOCK_SIZE], MAX_BLOCK_DATA_SIZE));
}
