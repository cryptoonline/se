//
// Blowfish.h
// BlindStorage
//

#ifndef __BlindStorage__Blowfish__
#define __BlindStorage__Blowfish__

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include <stdint.h>

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "cryptopp/blowfish.h"
//using CryptoPP::Blowfish;

#include "cryptopp/modes.h"
using CryptoPP::CTR_Mode;

class Blowfish {
private:
	byte* key;
	byte* plaintext;
	byte* ciphertext;

public:
	Blowfish();
	~Blowfish();
	byte* keyGen();
	byte* ENC(byte* plaintext, uint32_t size, byte* key, byte* iv);
	byte* DEC(byte* ciphertext, uint32_t size, byte* key, byte* iv);
};

#endif
