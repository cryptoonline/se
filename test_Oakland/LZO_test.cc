//
// LZO_test.cc
// Blind Storage
//

#include "gtest/gtest.h"

#include "../src/LZO.h"
#include <cstdlib>

TEST(LZOTest, Test1){
	LZO compressor;

	byte array[1024*1024];
	for(int i = 0; i < 1024*1024; i++)
		array[i] = rand();

	byte* compressedData;
	size_t compressedSize;
	compressor.compress(array, 1024*1024, compressedData, compressedSize);
	cout << "Size of compressed data is " << compressedSize << endl;
}
