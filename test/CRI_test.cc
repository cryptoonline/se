//
// CRI_test.cc
// BlindStorage Test
//

#include "gtest/gtest.h"

#include "./../src/CRI.h"
#include "./../src/Debug.h"

TEST(CRITest, Search_True){
	srand(clock());
	higherfid_t higherfid = rand();
	
	CRI cri(higherfid);
	
	prSubsetSize_t size = rand();
	prSubsetSeed_t seed = rand();
	byte lowerFid[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
										 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
										 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 
										 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B};

	byte lowerFidSearchKey[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
				 										 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
										 				 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 
										 				 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B};

	cri.addFile(size, seed, lowerFid);

	CRIBlock block;
	cri.search(lowerFidSearchKey, block);
	byte blockBytes[CRI_BLOCK_SIZE];
	block.get(blockBytes);
	
	printhex(blockBytes, CRI_BLOCK_SIZE, "BLOCK BYTES");

	EXPECT_TRUE(block.match(lowerFidSearchKey));
}

TEST(CRITest, Search_False){
	srand(clock());
	higherfid_t higherfid = rand();
	
	CRI cri(higherfid);
	
	prSubsetSize_t size = rand();
	prSubsetSeed_t seed = rand();
	byte lowerFid[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
										 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
										 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 
										 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B};

	byte lowerFidSearchKey[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
				 										 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
										 				 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 
										 				 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x11};

	cri.addFile(size, seed, lowerFid);

	CRIBlock block;
	cri.search(lowerFidSearchKey, block);
	byte blockBytes[CRI_BLOCK_SIZE];
	block.get(blockBytes);
	
	EXPECT_FALSE(block.match(lowerFidSearchKey));
}

TEST(CRITest, Search_MultipleCRIEntriesCase){
	srand(clock());
	higherfid_t higherfid = rand();
	
	CRI cri(higherfid);
	
	prSubsetSize_t size = rand();
	prSubsetSeed_t seed = rand();
	byte lowerFid[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
										 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
										 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 
										 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x00};

	byte lowerFidSearchKey[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
				 										 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
										 				 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 
										 				 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x00};

	lowerFidSearchKey[LOWERFID_SIZE-1] = rand()%256;

	for(int i = 0; i < 255; i++){
		lowerFid[LOWERFID_SIZE-1] = i;
		cri.addFile(size, seed, lowerFid);
	}

	CRIBlock block;
	cri.search(lowerFidSearchKey, block);
	byte blockBytes[CRI_BLOCK_SIZE];
	block.get(blockBytes);
	
	EXPECT_TRUE(block.match(lowerFidSearchKey));
}

TEST(CRIBytesTest, Test1){
	srand(clock());
	higherfid_t higherfid = rand();
	
	CRI cri(higherfid);
	
	prSubsetSize_t size = rand();
	prSubsetSeed_t seed = rand();
	byte sizeBytes[sizeof(prSubsetSize_t)];
	memcpy(sizeBytes, static_cast<byte*>(static_cast<void*>(&size)), sizeof(prSubsetSize_t));
	byte seedBytes[sizeof(prSubsetSeed_t)];
	memcpy(seedBytes, static_cast<byte*>(static_cast<void*>(&seed)), sizeof(prSubsetSeed_t));

	byte lowerFid[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
										 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
										 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 
										 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x00};

	vector<byte> blocksBytesActual;
	vector<byte> blocksBytesExpected;

	for(int i = 0; i < 255; i++){
		lowerFid[LOWERFID_SIZE-1] = i;
		blocksBytesActual.insert(blocksBytesActual.end(), &sizeBytes[0], &sizeBytes[sizeof(prSubsetSize_t)]);
		blocksBytesActual.insert(blocksBytesActual.end(), &seedBytes[0], &seedBytes[sizeof(prSubsetSeed_t)]);
		blocksBytesActual.insert(blocksBytesActual.end(), &lowerFid[0], &lowerFid[LOWERFID_SIZE]);
	}

	cri.parseBytes(blocksBytesActual);
	cri.makeBytes(blocksBytesExpected);

	bool is_equal = false;
	 if ( blocksBytesExpected.size() < blocksBytesActual.size() )
    is_equal = std::equal ( blocksBytesExpected.begin(), blocksBytesExpected.end(), blocksBytesActual.begin() );
  else
    is_equal = std::equal ( blocksBytesActual.begin(), blocksBytesActual.end(), blocksBytesExpected.begin() );

	EXPECT_TRUE(is_equal);
}

TEST(CRITest, EmptinessTest){
	srand(clock());
	higherfid_t higherfid = rand();
	
	CRI cri(higherfid);
	
	prSubsetSize_t size = 0;
	prSubsetSeed_t seed = 0;
	byte sizeBytes[sizeof(prSubsetSize_t)];
	memcpy(sizeBytes, static_cast<byte*>(static_cast<void*>(&size)), sizeof(prSubsetSize_t));
	byte seedBytes[sizeof(prSubsetSeed_t)];
	memcpy(seedBytes, static_cast<byte*>(static_cast<void*>(&seed)), sizeof(prSubsetSeed_t));

	byte lowerFid[LOWERFID_SIZE] = {0};

	vector<byte> blocksBytesActual;
	vector<byte> blocksBytesExpected;

	for(int i = 0; i < 255; i++){
		blocksBytesActual.insert(blocksBytesActual.end(), &sizeBytes[0], &sizeBytes[sizeof(prSubsetSize_t)]);
		blocksBytesActual.insert(blocksBytesActual.end(), &seedBytes[0], &seedBytes[sizeof(prSubsetSeed_t)]);
		blocksBytesActual.insert(blocksBytesActual.end(), &lowerFid[0], &lowerFid[LOWERFID_SIZE]);
	}

	printhex(blocksBytesActual, "BLOCKS ACTUAL");
	cri.parseBytes(blocksBytesActual);

	EXPECT_TRUE(cri.isEmpty());
}
