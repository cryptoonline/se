//
//  SHA256.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__SHA256__
#define __BlindStorage__SHA256__

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "cryptopp562/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptopp562/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp562/hmac.h"
using CryptoPP::HMAC;

#include "cryptopp562/sha.h"
using CryptoPP::SHA256;

#include "cryptopp562/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp562/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;
using CryptoPP::HashVerificationFilter;

#include "cryptopp562/secblock.h"
using CryptoPP::SecByteBlock;


class SHA256bit {
private:
    int16_t keyLength;
    unsigned char key[16];
        
    
    
public:
    
    SHA256bit();
    const unsigned char* keyGen();
    unsigned char* doFinal(string filename);
    bool verify(const unsigned char* hmac);
};

#endif /* defined(__BlindStorage__SHA256__) */
