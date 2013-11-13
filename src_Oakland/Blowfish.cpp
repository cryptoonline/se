//
// Blowfish.cpp
// BlindStorage
//

#include "Blowfish.h"

Blowfish::Blowfish(){
	key = new byte[CryptoPP::Blowfish::DEFAULT_KEYLENGTH];
}

Blowfish::~Blowfish(){
}

byte* Blowfish::keyGen(){
	cout << "Blowfish key size is " << CryptoPP::Blowfish::DEFAULT_KEYLENGTH << endl;
	CryptoPP::AutoSeededRandomPool prng;
	prng.GenerateBlock(key, CryptoPP::Blowfish::DEFAULT_KEYLENGTH);
	return key;
}

byte* Blowfish::ENC(byte* plaintext, uint32_t size, byte* key, byte* iv){
	ciphertext = new byte[size];
	CryptoPP::CTR_Mode<CryptoPP::Blowfish>::Encryption encryptor;
	encryptor.SetKeyWithIV(key, CryptoPP::Blowfish::DEFAULT_KEYLENGTH, iv);
	encryptor.ProcessData(ciphertext, plaintext, size);
	return ciphertext;
}

byte* Blowfish::DEC(byte* ciphertext, uint32_t size, byte* key, byte* iv){
	plaintext = new byte[size];
	CryptoPP::CTR_Mode<CryptoPP::Blowfish>::Decryption decryptor;
	decryptor.SetKeyWithIV(key, CryptoPP::Blowfish::DEFAULT_KEYLENGTH, iv);
	decryptor.ProcessData(plaintext, ciphertext, size);
	return plaintext;
}
