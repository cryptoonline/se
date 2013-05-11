//
//  AES.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/18/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__AES__
#define __BlindStorage__AES__

#include <iostream>
#include <stdint.h>
#include <math.h>

#include "cryptopp562/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptopp562/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp562/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp562/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "cryptopp562/aes.h"

#include "cryptopp562/ccm.h"
using CryptoPP::CTR_Mode;

#include "assert.h"

#include "Debug.h"

class AES {
private:
	int KEYLENGTH;
    
public:
    AES();
    ~AES();
    void keyGen(byte key[], int KEYLENGTH = CryptoPP::AES::DEFAULT_KEYLENGTH);
    void ENC_CTR(byte input[], byte output[], uint32_t size, byte key[], byte iv[]);
    void DEC_CTR(byte input[], byte output[], uint32_t size, byte key[], byte iv[]);
    void ENC_CBC(byte input[], byte output[], uint32_t size, byte key[], byte iv[]);
	void DEC_CBC(byte input[], byte output[], uint32_t size, byte key[], byte iv[]);
  };

#endif /* defined(__BlindStorage__AES__) */
