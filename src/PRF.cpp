//
//  PRF.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "PRF.h"
unsigned char* PRF::key = new unsigned char[16];

uint32_t PRF::encrypt(unsigned char* plaintext, uint16_t size, uint32_t bitMask){
    AES cipher;
	uint16_t ciphertextSize = 32;
	unsigned char* ciphertext = cipher.ENC(plaintext, size, key);
    uint32_t integer = *(uint32_t*)(ciphertext);
	return integer & bitMask; 	
}

unsigned char* PRF::keyGen(){
    AES cipher;
	key = cipher.keyGen();
	return key;
}
