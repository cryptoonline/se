// 
// FileStore_test.cc
// Searchable Encryption Test
//

#include "gtest/gtest.h"

#include "./../src/FileStore.h"

TEST(FileStoreCopyTest, Test1){
	FileStore store;
	string src = "/Users/naveed/BStore/data/FileStoreTest/1.";
	string dst = "/Users/naveed/BStore/data/FileStoreTest/10.";
	store.copy(src, dst);
}

TEST(FileStoreRemoveTest, Test1){
	FileStore store;
	string path = "/Users/naveed/BStore/data/FileStoreTest/2.";
	store.remove(path);
}

TEST(FileStoreExistenceCheck, Test1){
	string path0 = "/Users/naveed/BStore/data/FileStoreTest/20.";
	string path1 = "/Users/naveed/BStore/data/FileStoreTest/200.";
	FileStore store;
	EXPECT_TRUE(store.isFilePresent(path0));
	EXPECT_FALSE(store.isFilePresent(path1));
}
