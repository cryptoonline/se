//
// TBlock_test.cc
// BlindStorage Test
//

#include "gtest/gtest.h"

#include "./../src/TBlock.h"
#include "./../src/Key.h"
#include "./../src/parameters.h"
#include "./../src/Debug.h"

TEST(TBlockPreprocesing, Test1){
	TBlock tblock(0);
	
	Key key("TBlockTestKey", AES_KEY_SIZE);
	byte tblockKey[AES_KEY_SIZE];
	key.get(tblockKey);
	tblock.setKey(tblockKey);

	tblock.make(10, 1);
	byte blockExpected[] = {10, 0, 0, 0, 1, 0, 0, 0, 0, 0};

	byte blockActual[TBLOCK_SIZE];
	tblock.getDecrypted(blockActual);

	byte blockEncrypted[TBLOCK_SIZE];
	tblock.getEncrypted(blockEncrypted);
	
	EXPECT_TRUE( 0 == memcmp(blockExpected, blockActual, TBLOCK_SIZE));
}

TEST(TBlockOnline, Test1){
	TBlock tblock(0);

	Key key("TBlockTestKey", AES_KEY_SIZE);
	byte tblockKey[AES_KEY_SIZE];
	key.get(tblockKey);
	tblock.setKey(tblockKey);

	byte blockEncrypted[] = {0xb4, 0x73, 0xf2, 0x06, 0xae, 0x54, 0xba, 0x3c, 0x00, 0x00};
	tblock.parse(blockEncrypted);

	byte blockExpected[] = {10, 0, 0, 0, 1, 0, 0, 0, 0, 0};
	byte blockActual[TBLOCK_SIZE];
	tblock.getDecrypted(blockActual);

	EXPECT_TRUE( 0 == std::memcmp(blockExpected, blockActual, TBLOCK_SIZE));
}

TEST(TBlockOnline, size){
	TBlock tblock(0);

	Key key("TBlockTestKey", AES_KEY_SIZE);
	byte tblockKey[AES_KEY_SIZE];
	key.get(tblockKey);
	tblock.setKey(tblockKey);

	byte blockEncrypted[] = {0xb4, 0x73, 0xf2, 0x06, 0xae, 0x54, 0xba, 0x3c, 0x00, 0x00};
	tblock.parse(blockEncrypted);

	byte blockExpected[] = {10, 0, 0, 0, 1, 0, 0, 0, 0, 0};
	byte blockActual[TBLOCK_SIZE];
	tblock.getDecrypted(blockActual);

	prSubsetSize_t sizeExpected = 10;
	prSubsetSize_t sizeActual = tblock.getSize();

	EXPECT_EQ(sizeExpected, sizeActual);
}

TEST(TBlockOnline, seed){
	TBlock tblock(0);

	Key key("TBlockTestKey", AES_KEY_SIZE);
	byte tblockKey[AES_KEY_SIZE];
	key.get(tblockKey);
	tblock.setKey(tblockKey);

	byte blockEncrypted[] = {0xb4, 0x73, 0xf2, 0x06, 0xae, 0x54, 0xba, 0x3c, 0x00, 0x00};
	tblock.parse(blockEncrypted);

	byte blockExpected[] = {10, 0, 0, 0, 1, 0, 0, 0, 0, 0};
	byte blockActual[TBLOCK_SIZE];
	tblock.getDecrypted(blockActual);

	prSubsetSeed_t seedExpected = 1;
	prSubsetSeed_t seedActual = tblock.getSeed();

	EXPECT_EQ(seedExpected, seedActual);
}
