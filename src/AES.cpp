//
//  AES.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/18/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "AES.h"

AES::AES(){
	KEYLENGTH = AES_KEY_SIZE;//CryptoPP::AES::DEFAULT_KEYLENGTH;
}
    
AES::~AES(){
}

void AES::keyGen(byte key[], int KEYLENGTH){
	this->KEYLENGTH = KEYLENGTH;
	CryptoPP::AutoSeededRandomPool prng;
  prng.GenerateBlock(key, KEYLENGTH);
}
    
void AES::ENC_CTR(byte input[], byte output[], uint32_t size, byte key[], byte iv[]){
	CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption encryptor;
  encryptor.SetKeyWithIV(key, KEYLENGTH, iv);
  encryptor.ProcessData(output, input, size);
}

void AES::DEC_CTR(byte input[], byte output[], uint32_t size, byte key[], byte iv[]){
  CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption decryptor;
  decryptor.SetKeyWithIV(key, KEYLENGTH, iv);
  decryptor.ProcessData(output, input, size);
}

void AES::ENC_CBC(byte input[], byte output[], uint32_t size, byte key[], byte iv[]){
	CryptoPP::CBC_CTS_Mode<CryptoPP::AES>::Encryption encryptor;
	encryptor.SetKeyWithIV(key, KEYLENGTH, iv);
	encryptor.ProcessData(output, input, size);
}

void AES::DEC_CBC(byte input[], byte output[], uint32_t size, byte key[], byte iv[]){
	CryptoPP::CBC_CTS_Mode<CryptoPP::AES>::Decryption decryptor;
	decryptor.SetKeyWithIV(key, KEYLENGTH, iv);
	decryptor.ProcessData(output, input, size);
}
