//
//  HashMAC.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "HashMAC.h"

//byte HashMAC::key[HMAC_KEY_SIZE];

void HashMAC::keyGen(byte key[]){
	AutoSeededRandomPool prng;
  prng.GenerateBlock(key, HMAC_KEY_SIZE);
//	memcpy(key, this->key, HMAC_KEY_SIZE);
}

void HashMAC::setKey(byte key[]){
	memcpy(this->key, key, HMAC_KEY_SIZE);
}

void HashMAC::doFinal(string filename, byte mac[]){
	CryptoPP::HMAC<CryptoPP::SHA256>(key,HMAC_KEY_SIZE).
	CalculateDigest(mac, (byte*)filename.c_str(), filename.length());
}
