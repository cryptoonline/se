//
// fileID_test.cc
// BlindStorage Test
//

#include <stdlib.h>

#include "gtest/gtest.h"
#include "./../src/parameters.h"
#include "./../src/fileID.h"
#include "./../src/Debug.h"

TEST(FileID, Test1NeedsManualChecking) {
	fileID fid("Hi There");
	byte fidBytes[FILEID_SIZE];
	fid.get(fidBytes);
	printhex(fidBytes, FILEID_SIZE, "FID Bytes");
}

TEST(FileID, ZeroBytes) {
	byte fidBytesInput[FILEID_SIZE] = {0};
	fileID fid(fidBytesInput);

	byte fidBytesOutput[FILEID_SIZE];
	fid.get(fidBytesOutput);

	EXPECT_TRUE( 0 == std::memcmp(fidBytesOutput, fidBytesInput, FILEID_SIZE));
}

TEST(FileID, NonZeroBytes) {
	srand(clock());
	byte fidBytesInput[FILEID_SIZE];
	for( int i = 0; i < FILEID_SIZE; i++)
		fidBytesInput[i] = rand();
	fileID fid(fidBytesInput);

	byte fidBytesOutput[FILEID_SIZE];
	fid.get(fidBytesOutput);

	EXPECT_TRUE( 0 == std::memcmp(fidBytesOutput, fidBytesInput, FILEID_SIZE));
}
