//
// optBStore.cpp
// BlindStorage
//

#include "optBStore.h"
optBStore::optBStore()//:D(10)
{
}

optBStore::~optBStore(){
}

void optBStore::add(string filename, byte fileBytes[], size_t size){
	b_index_t numBlocks = (b_index_t)ceil((double)size/(double)MAX_BLOCK_DATA_SIZE);

//	fileID fid(filename);
//	PRSubset prSubset(numBlocks*BLOW_UP);

//	T.addFile(filename, size, prSubset);
//	D.addFile(fileBytes, size, fid, prSubset);
}

void optBStore::finalize(){
//	D.encryptEmptyBlocks();
//	D.writeToDisk();
//	T.writeToDisk();
}
