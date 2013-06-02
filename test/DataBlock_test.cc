//
// DataBlock_test.cc
// BlindStorage
//

#include "gtest/gtest.h"

#include "./../src/DataBlock.h"
#include "./../src/fileID.h"

class DataBlockPreprocessing : public testing::Test {
	protected:
		virtual void SetUp(){

		}

		virtual void TearDown(){
		}

//		void makeDataBlock(b_index_t index, fileID fid, bool isCRI, byte block[], int dataSize){
//			this->index = index;
//			this->fid = fid;
//			this->isCRI = isCRI;
//			this->block = block;
//			this->dataSize = dataSize;

//			dataBlock.make(fid, block, dataSize, false, 0);
//		}

//		b_index_t index;
//		version_t version;
//		fileID fid;
//		bool isPadded;
//		bool isCRI;
//		byte* block;
//		int dataSize;

		DataBlock dataBlock;
};

TEST_F(DataBlockPreprocessing, NoPadding){
}

TEST_F(DataBlockPreprocessing, Padding){
}

TEST_F(DataBlockPreprocessing, CRI){
}
