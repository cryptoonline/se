//
// LZO.h
// Blind Storage
//

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using std::cout;
using std::endl;

#include <cstring>

#include "lzo/lzoconf.h"
#include "lzo/lzodefs.h"
#include "lzo/lzo1x.h"

#include "parameters.h"

#include "Debug.h"

#ifndef __BlindStorage__LZO__
#define __BlindStorage__LZO__

class LZO{

public:
	LZO();
	void compress(byte* inputData, size_t size, byte* outputData, size_t& compressedSize);
	void decompress(byte* inputData, size_t size, byte* outputData, size_t& decompressedSize);

	void addHeader(byte* data, uint32_t size);
	uint32_t getSizeFromHeader(byte* data);

};
#endif /* (defined __BlindStorage__LZO__) */
