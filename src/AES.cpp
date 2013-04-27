//
//  AES.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/18/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "AES.h"

//
//  AES.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/15/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

AES::AES(){
    key = new byte[CryptoPP::AES::DEFAULT_KEYLENGTH];
}
    
AES::~AES(){
}


byte* AES::keyGen(){
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
//    ::memset( key, 0x01, CryptoPP::AES::DEFAULT_KEYLENGTH );
    return key;
}
    
byte* AES::ENC(byte* plaintext, uint32_t size, byte* key, byte* iv){
    ciphertext = new byte[size];
    CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption Encryptor;
    Encryptor.SetKeyWithIV(key, 16, iv);
    Encryptor.ProcessData(ciphertext, plaintext, size);
    return ciphertext;
}

byte* AES::DEC(byte* ciphertext, uint32_t size, byte* key, byte* iv){
    plaintext = new byte[size];
    if(ciphertext == NULL)
        cerr << "Ciphertext is NULL in " << __PRETTY_FUNCTION__ << " in file " << __FILE__ << " at line " << __LINE__;
    CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption Decryptor;
    Decryptor.SetKeyWithIV(key, 16, iv);
    Decryptor.ProcessData(plaintext, ciphertext, size);
    return plaintext;
}

void AES::print(string tag, string value){
    cout << endl << "<" << tag << ">" << value << "</" << tag << ">" << endl;
}

void AES::print(string tag, byte* value){
    cout << endl << "<" << tag << ">" << value << "</" << tag << ">" << endl;
}


