//
// TruncAES.h
// BlindStorage
//

#ifndef __BlindStorage__TruncAES__
#define __BlindStorage__TruncAES__

#include "parameters.h"
#include "AES.h"

class TruncAES {
private:
	AES cipher;

public:
	void keyGen(byte key[]);
	void ENC(byte input[], byte output[], int size, byte key[], byte iv[]);
	void DEC(byte input[], byte output[], int size, byte key[], byte iv[]);
};
#endif /*defined (__BlindStorage__TruncAES__)*/
