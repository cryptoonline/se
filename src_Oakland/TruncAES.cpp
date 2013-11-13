//
// TruncAES.cpp
// BlindStorage
//

#include "TruncAES.h"

void TruncAES::keyGen(byte key[]){
	cipher.keyGen(key);
}

void TruncAES::ENC(byte input[], byte output[], int size,byte key[], byte iv[]) {
	byte paddedInput[AES_BLOCK_SIZE] = {0};
	byte paddedOutput[AES_BLOCK_SIZE];
	memcpy(paddedInput, input, size);
	cipher.ENC_CTR(paddedInput, paddedOutput, AES_BLOCK_SIZE, key, iv);
	memcpy(output, paddedOutput, size);
}

void TruncAES::DEC(byte input[], byte output[], int size, byte key[], byte iv[]) {
	byte paddedInput[AES_BLOCK_SIZE] = {0};
	byte paddedOutput[AES_BLOCK_SIZE];
	memcpy(paddedInput, input, size);
	cipher.DEC_CTR(paddedInput, paddedOutput, AES_BLOCK_SIZE, key, iv);
	memcpy(output, paddedOutput, size);
}
