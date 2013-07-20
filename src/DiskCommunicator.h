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
using std::ios_base;
using std::ios;

#include <sys/stat.h>

#include <boost/iostreams/device/mapped_file.hpp>

#include "parameters.h"
#include "Debug.h"

#define DSize (TOTAL_BLOCKS*BLOCK_SIZE)
#define TSize (TTOTAL_BLOCKS*TBLOCK_SIZE)

class DiskCommunicator{
private:
	boost::iostreams::mapped_file_source DSource;
	boost::iostreams::mapped_file_sink DSink;
	boost::iostreams::mapped_file_source TSource;
	boost::iostreams::mapped_file_sink TSink;

public:
	DiskCommunicator();
	~DiskCommunicator();
	void dPut(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks);
	void dGet(b_index_t* blockLocations, b_index_t numBlocks, byte* blocks);
	void tPut(t_index_t index, byte* block);
	void tGet(t_index_t index, byte* block);
};

#endif /*defined(__BlindStorage__DiskCommunicator__)*/
