//
// LZO.cpp
// Blind Storage
//

#include "LZO.h"

LZO::LZO(){
}

void LZO::addHeader(byte* data, uint32_t size){
	memcpy(data, static_cast<byte*>(static_cast<void*>(&size)), sizeof(uint32_t));
}

uint32_t LZO::getSizeFromHeader(byte* data){
	return *(uint32_t*)(data);
}

void LZO::compress(byte* inputData, size_t size, byte* outputData, size_t& compressedSize){

	if(lzo_init() != LZO_E_OK){
		cout << "LZO initialization failed." << endl;
		exit(1);
	}

	byte* workingMemory = (byte*)malloc(LZO1X_1_11_MEM_COMPRESS);
//	outputData = new byte[size];//(byte*)malloc(size);
	
	lzo1x_1_11_compress(inputData, size, outputData+COMPRESSION_HEADER_SIZE, &compressedSize, workingMemory);

	free(workingMemory);

}

void LZO::decompress(byte* inputData, size_t size, byte* outputData, size_t& decompressedSize){

	if(lzo_init() != LZO_E_OK){
		cout << "LZO initialization failed." << endl;
		exit(1);
	}

	lzo1x_decompress(inputData+COMPRESSION_HEADER_SIZE, size, outputData, &decompressedSize, NULL);
}
