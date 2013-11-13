//
//  HashMAC.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__HashMAC__
#define __BlindStorage__HashMAC__

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp/hmac.h"
using CryptoPP::HMAC;

#include "cryptopp/sha.h"
using CryptoPP::SHA256;

#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;
using CryptoPP::HashVerificationFilter;

#include "cryptopp/secblock.h"
using CryptoPP::SecByteBlock;

#include "parameters.h"

class HashMAC {
private:
	byte key[HMAC_KEY_SIZE];
    
public: 
	void keyGen(byte key[]);
	void setKey(byte key[]);
	void doFinal(string filename, byte mac[]);
};

#endif /* defined(__BlindStorage__SHA256__) */
