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
