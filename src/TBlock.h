//
// TBlock.h
// BlindStorage
//

#ifndef __BlindStorage__TBlock__
#define __BlindStorage__TBlock__

#include <iostream>
using std::memcpy;
using std::memset;

#include "parameters.h"
#include "TruncAES.h"

class TBlock {
private:
	static t_index_t instanceCounter;
	t_index_t index;

	version_t version;
	prSubsetSize_t size;
	prSubsetSeed_t seed;
	
	byte block[TBLOCK_SIZE];

	static byte key[AES_KEY_SIZE];
	byte iv[AES_BLOCK_SIZE];

	bool isBlockEncrypted;
	void encrypt();
	void decrypt();
	void makeIV();

public:
	TBlock();
	TBlock(t_index_t index);
	~TBlock();
	void setKey(byte key[]);
	void make(prSubsetSize_t size, prSubsetSeed_t seed);
	void parse(byte block[]);
	void update(prSubsetSize_t size, prSubsetSeed_t seed);

	bool isOccupied();
	void encryptIfEmpty();

	void getEncrypted(byte block[]);
	void getDecrypted(byte block[]);

	prSubsetSize_t getSize();
	prSubsetSeed_t getSeed();
};

#endif /* defined(__BlindStorage__TBlock__) */
