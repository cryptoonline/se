// 
// FileStore_test.cc
// Searchable Encryption Test
//

#include "gtest/gtest.h"

#include "./../src/FileStore.h"

TEST(FileStoreGetTest, Test1){
	FileStore store;
	string src = "/Users/naveed/BStore/data/FileStoreTest/1.";
	string dst = "/Users/naveed/BStore/data/FileStoreTest/10.";
	store.get(src, dst);
}

TEST(FileStorePutTest, Test1){
	FileStore store;
	string src = "/Users/naveed/BStore/data/FileStoreTest/2.";
	string dst = "/Users/naveed/BStore/data/FileStoreTest/20.";
	store.get(src, dst);
}

TEST(FileStoreRemoveTest, Test1){
	FileStore store;
	string path = "/Users/naveed/BStore/data/FileStoreTest/2.";
	store.remove(path);
}
