//
// DataStructures.h
// BlindStorage
//

#ifndef __BlindStorage__DataStructures__
#define __BlindStorage__DataStructures__

#include <iostream>
using std::string;
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include "parameters.h"

class DataStructures{
private:
	static char* T;
	static char* D;
	int64_t TSize;
	int64_t DSize;

	int64_t readFileSize(string filename);
	void loadFile(string filename, char* input, int64_t size);
	
public:
	DataStructures();
	void dPut(uint32_t* blockLocations, uint32_t numBlocks, unsigned char** blocks);
	void dGet(uint32_t* blockLocations, uint32_t numBlocks, unsigned char** blocks);
	void tPut(uint32_t index, unsigned char* block);
	void tGet(uint32_t index, unsigned char* block);
};
#endif
