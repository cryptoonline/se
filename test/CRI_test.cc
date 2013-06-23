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

	int numZeros = MAX_BLOCK_DATA_SIZE-CRI_PER_BLOCK*CRI_BLOCK_SIZE+TRAILER_SIZE;
	byte zeroArray[numZeros];
	std::memset(zeroArray, 0, numZeros);

	for(int i = 0; i < 2*CRI_PER_BLOCK+3; i++){
		if(i % CRI_PER_BLOCK == 0 && i!=0){
			blocksBytesActual.insert(blocksBytesActual.end(), &zeroArray[0], &zeroArray[numZeros]);
			cout << "Entered" << endl;
		}
		lowerFid[LOWERFID_SIZE-1] = i;
		blocksBytesActual.insert(blocksBytesActual.end(), &sizeBytes[0], &sizeBytes[sizeof(prSubsetSize_t)]);
		blocksBytesActual.insert(blocksBytesActual.end(), &seedBytes[0], &seedBytes[sizeof(prSubsetSeed_t)]);
		blocksBytesActual.insert(blocksBytesActual.end(), &lowerFid[0], &lowerFid[LOWERFID_SIZE]);
		cout << "Size is " << blocksBytesActual.size() << endl;
	}

	cri.parseBytes(blocksBytesActual.data(), blocksBytesActual.size());
	vector<byte> blocksBytesExpected; 
	blocksBytesExpected.reserve((int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOCK_SIZE);
	cri.makeBytes(blocksBytesExpected);

	bool is_equal = true;
	int pointer = 0;
	for(int i = 0; i < 2*CRI_PER_BLOCK+3; i++){
		if(i % CRI_PER_BLOCK == 0 && i!=0)
			pointer += MAX_BLOCK_DATA_SIZE - CRI_BLOCK_SIZE * CRI_PER_BLOCK + TRAILER_SIZE;
		cout << "pointer is " << pointer << endl; 
		vector<byte> blockExpected;
		vector<byte> blockActual;
		blockExpected.insert(blockExpected.begin(), blocksBytesExpected.begin()+pointer, blocksBytesExpected.begin()+pointer+CRI_BLOCK_SIZE);
		blockActual.insert(blockActual.begin(), blocksBytesActual.begin()+pointer, blocksBytesActual.begin()+pointer+CRI_BLOCK_SIZE);
		is_equal &= std::equal(blockExpected.begin(), blockExpected.end(), blockActual.begin());
		printhex(blockExpected, "EXPECTED");
		printhex(blockActual, "ACTUAL");
		if(is_equal)
			cout << "true" << endl;
		else
			cout << "false" << endl;
		pointer += CRI_BLOCK_SIZE;
		
	}

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
	vector<byte> blocksBytesExpected((int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOCK_SIZE);

	for(int i = 0; i < CRI_PER_BLOCK; i++){
		blocksBytesActual.insert(blocksBytesActual.end(), &sizeBytes[0], &sizeBytes[sizeof(prSubsetSize_t)]);
		blocksBytesActual.insert(blocksBytesActual.end(), &seedBytes[0], &seedBytes[sizeof(prSubsetSeed_t)]);
		blocksBytesActual.insert(blocksBytesActual.end(), &lowerFid[0], &lowerFid[LOWERFID_SIZE]);
	}

	cri.parseBytes(blocksBytesActual.data(), blocksBytesActual.size());
	
	EXPECT_TRUE(cri.isEmpty());
}

TEST(CRITest, NonEmptinessTest){
	srand(clock());
	higherfid_t higherfid = rand();
	
	CRI cri(higherfid);
	
	prSubsetSize_t size = 0;
	prSubsetSeed_t seed = 0;
	byte sizeBytes[sizeof(prSubsetSize_t)];
	memcpy(sizeBytes, static_cast<byte*>(static_cast<void*>(&size)), sizeof(prSubsetSize_t));
	byte seedBytes[sizeof(prSubsetSeed_t)];
	memcpy(seedBytes, static_cast<byte*>(static_cast<void*>(&seed)), sizeof(prSubsetSeed_t));

	byte lowerFid[LOWERFID_SIZE] = {1};

	vector<byte> blocksBytesActual;
	vector<byte> blocksBytesExpected((int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOCK_SIZE);

	for(int i = 0; i < CRI_PER_BLOCK; i++){
		blocksBytesActual.insert(blocksBytesActual.end(), &sizeBytes[0], &sizeBytes[sizeof(prSubsetSize_t)]);
		blocksBytesActual.insert(blocksBytesActual.end(), &seedBytes[0], &seedBytes[sizeof(prSubsetSeed_t)]);
		blocksBytesActual.insert(blocksBytesActual.end(), &lowerFid[0], &lowerFid[LOWERFID_SIZE]);
	}

	cri.parseBytes(blocksBytesActual.data(), blocksBytesActual.size());
	
	EXPECT_FALSE(cri.isEmpty());
}
