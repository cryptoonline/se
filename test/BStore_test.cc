//
// BStore_test.cc
// BlindStorage Test
//

#include "gtest/gtest.h"
#include "./../src/BStore.h"

#include <string>
using std::string;

TEST(PreprocessingTest, Test1){
	string directoryPath = "/Users/naveed/BStore/datasets/testdir";
	BStore store(directoryPath);
}
