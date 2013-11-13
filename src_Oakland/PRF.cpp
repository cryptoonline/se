//
//  PRF.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "PRF.h"

byte PRF::key[PRF_KEY_SIZE];

uint32_t PRF::encrypt(byte plaintext[], uint32_t bitMask){
	AES cipher;
	byte ciphertext[SHA_BLOCK_SIZE];

	memset(iv, 0, AES_BLOCK_SIZE);

	cipher.ENC_CBC(plaintext, ciphertext, SHA_BLOCK_SIZE, key, iv);
	uint32_t integer = *(uint32_t*)(ciphertext);
	return integer & bitMask; 	
}

void PRF::setKey(byte key[]){
	memcpy(this->key, key, AES_KEY_SIZE);
}

void PRF::setIV(byte iv[]){
	memcpy(this->iv, iv, AES_BLOCK_SIZE);
}

void PRF::keyGen(byte key[]){
  AES cipher;
	cipher.keyGen(key);
	memcpy(key, this->key, PRF_KEY_SIZE);
}
