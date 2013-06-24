//
// DataBlock_test.cc
// BlindStorage
//

#include "gtest/gtest.h"

#include "./../src/helper.h"
#include "./../src/DataBlock.h"
#include "./../src/fileID.h"
#include "./../src/Key.h"
#include "./../src/Debug.h"

class DataBlockTest : public testing::Test {
protected:
	virtual void SetUp(){
		dataSize = MAX_BLOCK_DATA_SIZE;
		memset(blockData, 0, BLOCK_SIZE);
		memset(randomData, 0, dataSize);
		
		Key key("DataBlockTestKey", AES_KEY_SIZE);
		byte blockKey[AES_KEY_SIZE];
		key.get(blockKey);
		block.setKey(blockKey);
		
		gen_random(randomData, dataSize);

		fileID fid("test test");
		byte fidBytes[FILEID_SIZE];
		fid.get(fidBytes);
		
		memcpy(blockData, randomData, dataSize);
		blockData[dataSize] = 1;
		blockData[dataSize+1] = 1;
		memcpy(&blockData[dataSize+2], fidBytes, FILEID_SIZE);

		DataBlock block(0);
		this->block = block;
		block.make(fid, randomData, dataSize, true);
	
		memset(encryptedBlock, 0, BLOCK_SIZE);
		memset(decryptedBlock, 0, BLOCK_SIZE);

		block.getEncrypted(encryptedBlock);
		block.getDecrypted(decryptedBlock);
	}

	virtual void TearDown(){
	}

	DataBlock block;
	byte blockData[BLOCK_SIZE];

	byte encryptedBlock[BLOCK_SIZE];
	byte decryptedBlock[BLOCK_SIZE];

	byte randomData[BLOCK_SIZE];

	int dataSize;
	

};

TEST_F(DataBlockTest, Preprocessing) {
 	EXPECT_TRUE( 0 == std::memcmp(blockData, decryptedBlock, BLOCK_SIZE));
}

TEST_F(DataBlockTest, OnlineRead) {

	DataBlock block(0);
	
	Key key("DataBlockTestKey", AES_KEY_SIZE);
	byte blockKey[AES_KEY_SIZE];
	key.get(blockKey);
	block.setKey(blockKey);
	
	block.parse(encryptedBlock);
	byte blockDataRetrieved[BLOCK_SIZE];
	block.getDecrypted(blockDataRetrieved);

	EXPECT_TRUE( 0 == std::memcmp(blockDataRetrieved, blockData, BLOCK_SIZE));
}

TEST_F(DataBlockTest, OnlineUpdate){
	DataBlock block(0);
	byte blockDataUpdated[BLOCK_SIZE];
	memcpy(blockDataUpdated, blockData, BLOCK_SIZE);

	block.parse(encryptedBlock);
	
	blockDataUpdated[dataSize+1] = 0;
	blockDataUpdated[BLOCK_SIZE-2] = 1;
	fileID fid2("test test");

	byte blockDataExpected[BLOCK_SIZE];
	memcpy(blockDataExpected, blockDataUpdated, BLOCK_SIZE);
	block.update(fid2, blockDataUpdated, MAX_BLOCK_DATA_SIZE, false);

	byte blockDataActual[BLOCK_SIZE];
	block.getDecrypted(blockDataActual);

	EXPECT_TRUE( 0 == std::memcmp(blockDataExpected, blockDataActual, BLOCK_SIZE));

}

TEST_F(DataBlockTest, EmptyBlockTest) {
	DataBlock block(0);
	
	byte emptyBlock[BLOCK_SIZE] = {0};
	block.encryptIfEmpty(emptyBlock);
	
	byte decryptedBlock[BLOCK_SIZE];
	block.getDecrypted(decryptedBlock);

	EXPECT_TRUE( 0 == std::memcmp(decryptedBlock, emptyBlock, BLOCK_SIZE));
}

TEST_F(DataBlockTest, FidMatchTestCase){
	fileID fid1("test test");
	fileID fid2("test test");
	block.make(fid1, randomData, dataSize, true);
	EXPECT_TRUE(block.fidMatchCheck(fid2));
}

TEST_F(DataBlockTest, FidNonMatchTestCase){
	fileID fid1("test test");
	fileID fid2("test tess");
	block.make(fid1, randomData, dataSize, true);
	EXPECT_FALSE(block.fidMatchCheck(fid2));
}
