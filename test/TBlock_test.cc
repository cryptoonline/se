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
	
	Key key("data/keys/T_Test.KEY", AES_KEY_SIZE);
	byte tblockKey[AES_KEY_SIZE];
	key.get(tblockKey);
	tblock.setKey(tblockKey);

	tblock.make(10, 1);
	byte blockExpected[] = {10, 0, 0, 0, 1, 0, 0, 0, 0, 0};

	byte blockActual[TBLOCK_SIZE];
	tblock.getDecrypted(blockActual);

	byte blockEncrypted[TBLOCK_SIZE];
	tblock.getEncrypted(blockEncrypted);

	printhex(blockEncrypted, TBLOCK_SIZE, "Encrypted");
	EXPECT_TRUE( 0 == memcmp(blockExpected, blockActual, TBLOCK_SIZE));
}

TEST(TBlockOnline, Test1){
	TBlock tblock(0);

	Key key("data/keys/T_Test.KEY", AES_KEY_SIZE);
	byte tblockKey[AES_KEY_SIZE];
	key.get(tblockKey);
	tblock.setKey(tblockKey);
	byte blockEncrypted[] = {0x17, 0xc4, 0xc0, 0x8d, 0x8b, 0xbc, 0x5a, 0x12, 0x00, 0x00};
	tblock.parse(blockEncrypted);

	byte blockExpected[] = {10, 0, 0, 0, 1, 0, 0, 0, 0, 0};
	byte blockActual[TBLOCK_SIZE];
	tblock.getDecrypted(blockActual);

	EXPECT_TRUE( 0 == std::memcmp(blockExpected, blockActual, TBLOCK_SIZE));
}

TEST(TBlockOnline, size){
	TBlock tblock(0);

	Key key("data/keys/T_Test.KEY", AES_KEY_SIZE);
	byte tblockKey[AES_KEY_SIZE];
	key.get(tblockKey);
	tblock.setKey(tblockKey);

	byte blockEncrypted[] = {0x17, 0xc4, 0xc0, 0x8d, 0x8b, 0xbc, 0x5a, 0x12, 0x00, 0x00};
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

	Key key("data/keys/T_Test.KEY", AES_KEY_SIZE);
	byte tblockKey[AES_KEY_SIZE];
	key.get(tblockKey);
	tblock.setKey(tblockKey);

	byte blockEncrypted[] = {0x17, 0xc4, 0xc0, 0x8d, 0x8b, 0xbc, 0x5a, 0x12, 0x00, 0x00};
	tblock.parse(blockEncrypted);

	byte blockExpected[] = {10, 0, 0, 0, 1, 0, 0, 0, 0, 0};
	byte blockActual[TBLOCK_SIZE];
	tblock.getDecrypted(blockActual);

	prSubsetSeed_t seedExpected = 1;
	prSubsetSeed_t seedActual = tblock.getSeed();

	EXPECT_EQ(seedExpected, seedActual);
}

TEST(TBlockArray, Test1){
	TBlock tblocks[16];

	Key key("data/keys/T_Test.KEY", AES_KEY_SIZE);
	byte tblockKey[AES_KEY_SIZE];
	key.get(tblockKey);
	tblocks[0].setKey(tblockKey);

	byte blockEncrypted[] = {0xc4, 0x13, 0xc2, 0xc6, 0xf2, 0x3c, 0x33, 0x85, 0x00, 0x00};
	tblocks[0].parse(blockEncrypted);

	byte blockExpected[] = {10, 0, 0, 0, 1, 0, 0, 0, 0, 0};
	byte blockActual[TBLOCK_SIZE];
	tblocks[0].getDecrypted(blockActual);

	printhex(blockExpected, TBLOCK_SIZE, "Expected Block");
	printhex(blockActual, TBLOCK_SIZE, "Actual Block");
	EXPECT_TRUE( 0 == std::memcmp(blockExpected, blockActual, TBLOCK_SIZE));
}
