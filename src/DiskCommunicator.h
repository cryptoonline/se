//
// DiskCommunicator.h
// BlindStorage
//

#ifndef __BlindStorage__DiskCommunicator__
#define __BlindStorage__DiskCommunicator__

#include <iostream>
using std::string;
using std::cout;
using std::endl;
using std::memset;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <sys/stat.h>

#include "parameters.h"
#include "Debug.h"

class DiskCommunicator{
private:
	byte* T;
	byte* D;
	size_t TSize;
	size_t DSize;

	size_t readFileSize(string filename);
	void loadFile(string filename, byte* input, size_t size);
	
public:
	DiskCommunicator();
	~DiskCommunicator();
	void dPut(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks);
	void dGet(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks);
	void tPut(t_index_t index, byte* block);
	void tGet(t_index_t index, byte* block);

	void writeToDisk();

	void printD(string TAG);
	void printT(string TAG);
};

#endif /*defined(__BlindStorage__DiskCommunicator__)*/
