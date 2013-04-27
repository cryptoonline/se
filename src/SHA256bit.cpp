//
//  SHA256.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "SHA256bit.h"

SHA256bit::SHA256bit(){
    keyLength = 16;
}

const unsigned char * SHA256bit::keyGen(){
    AutoSeededRandomPool prng;
    prng.GenerateBlock(key, keyLength);
    return key;
}

unsigned char* SHA256bit::doFinal(string filename){
//    unsigned char* hash = new unsigned char[CryptoPP::SHA256::DIGESTSIZE];
    unsigned char* hmac = new unsigned char[CryptoPP::SHA256::DIGESTSIZE];
    unsigned char* hash = new unsigned char[CryptoPP::SHA256::DIGESTSIZE]();
    
//    CryptoPP::SHA().CalculateDigest(hash, (unsigned char*)filename.c_str(), 4);
    
   
        
    CryptoPP::HMAC<CryptoPP::SHA256>(key,keyLength).
    CalculateDigest(hmac, (unsigned char*)filename.c_str(), filename.length());
    
//    for(int i = 0; i < 32; i++)
//        cout << (int)hmac[i];
    
    return hmac;
}

bool verify(const unsigned char* hmac){
//    try
//	{
//		HMAC< SHA256 > hmac(key, key.size());
//		const int flags = HashVerificationFilter::THROW_EXCEPTION | HashVerificationFilter::HASH_AT_END;
//        
//		// Tamper with message
//		// plain[0] ^= 0x01;
//        
//		// Tamper with MAC
//		// mac[0] ^= 0x01;
//        
//		StringSource(plain + mac, true,
//                     new HashVerificationFilter(hmac, NULL, flags)
//                     ); // StringSource
//        
//		cout << "Verified message" << endl;
//	}
//	catch(const CryptoPP::Exception& e)
//	{
//		cerr << e.what() << endl;
//		exit(1);
//	}

}