//
// BStore_test.cc
// BlindStorage Test
//

#include "gtest/gtest.h"
#include "./../../src_Oakland/BStore.h"

#include <string>
using std::string;

TEST(PreprocessingTest, Test1){
	string directoryPath = "datasets/testdir";
	BStore* store = new BStore(directoryPath);
	delete store;
}
