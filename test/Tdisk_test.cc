//
// Tdisk.cpp
// BlindStorage Test
//

#include "gtest/gtest.h"
#include "./../src/Tdisk.h"
#include "./../src/TBlock.h"
#include "./../src/Ddisk.h"
#include "./../src/parameters.h"

TEST(TdiskTest, Test1){
	Tdisk T;
	
//	srand(clock());
//	byte fidBytesArray[512][FILEID_SIZE];
//	for(int i = 0; i < 512; i++){
//		byte fidBytes[FILEID_SIZE] = {0};//{rand()%0xFF, rand()%0xFF, rand()%0xFF, rand()%0xFF};
//		std::memcpy(fidBytesArray[i], fidBytes, FILEID_SIZE);
//		fileID fid(fidBytes);
//		for(int j = 0; j < 32; j++){
//			PRSubset prSubset(1024, 10);
//			T.addFile(fid, prSubset);
//		}
//	}

	byte fidBytes[FILEID_SIZE] = {0x01, 0x00, 0x00, 0x00, 0x11, 0x11};
	fileID fid(fidBytes);
	PRSubset prSubset(1024, 10);
	cout << "PRSubset size is " << prSubset.getSize() << " seed is " << prSubset.getSeed() << endl;

	T.addFile(fid, prSubset);
	
//	byte fidBytess[FILEID_SIZE] = {0x02, 0x00, 0x00, 0x00, 0x11, 0x11};
//	fileID fid2(fidBytess);

//	T.addFile(fid2, prSubset);

	Ddisk D;
	T.finalize(D);
	D.encryptEmptyBlocks();
	TBlock block;	
	T.getBlock(1, block);
	cout << "TBlock size is " << block.getSize() << " seed is " << block.getSeed() << endl;

	byte blockData[TBLOCK_SIZE];
	block.getDecrypted(blockData);
	printhex(blockData, TBLOCK_SIZE, "TBLOCK DATA");
//	EXPECT_EQ(1024, block.getSize());
//	EXPECT_EQ(10, block.getSeed());

}
