//
// PRSubset_test.cc
// BlindStorage Test
//

#include "gtest/gtest.h"
#include "./../../src_Oakland/PRSubset.h"
#include "./../../src_Oakland/Debug.h"

TEST(PRSubsetTest, Test1){
	PRSubset subset(1024, "test2");
	b_index_t set[1024];

	subset.get(set, 1024);
	printdec(set, 1024, "PRSubset Test1");
}

TEST(PRSubsetTest, Test2){
	PRSubset subset(1024, "test");
	b_index_t set[1024];

	subset.get(set, 1024);
	printdec(set, 1024, "PRSubset Test2");
}
