//
//  main.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "DataBlock.h"
#include "AES.h"
#include "Ddisk.h"
#include "parameters.h"
#include "fileID.h"
#include <ext/hash_map>

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

void print(string tag, unsigned char* value){
    cout << "<" << tag << ">" << value << "</" << tag << ">" << endl;
}

void print(string tag, char* value, uint32_t size){
    cout << endl << "*********************" << tag << " (start)*********************" << endl;
    for( int i = 0; i < size; i++)
        cout << value[i];
    cout << endl << "**********************" << tag << " (end)**********************" << endl;
}

int main(int argc, const char * argv[])
{
    
    byte data[5][32] = {
        {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f'},
        {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f'},
        {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f'},
        {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f'},
        {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f'}
    };
    
    
    char dataS[5*32] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','a', 'b', 'c', 'd', 'e', 'f'
    };
    
//    for(int i = 0; i < 5; i++){
//        //        cout << "Start address: " << &fileBytes << "\tEnd address: " << &fileBytes + i*MAX_BLOCK_DATA_SIZE << endl;
//        print("READ", &dataS[i*32], 32);
//    }

    SHA256bit sha;
    unsigned char* hash2 = sha.doFinal("test");
    unsigned char hash[32];
    unsigned char input[] = {'h', 'e', 'l', 'l', 'o'};
    string hello = "hello";
    CryptoPP::SHA256().CalculateDigest(hash, (unsigned char*)hello.c_str(), 5);
//    for(int i = 0; i < 32; i++)
//        cout << (int)hash2[i] << "\t";
    
    
    
    Communicator comm;
    Ddisk* D = new Ddisk(comm);
    
    std::ifstream file("/Users/naveed/Desktop/test");

    uint32_t* subset = new uint32_t[2048];
    
    for(int i = 0; i < 2048; i++){
        subset[i] = i;
        
    }
    
//    fileID* fid = new fileID("test");
    PRSubset prSubset(1000, 10);

    for(int i = 0; i < 10; i++)
        cout << prSubset.get()[i] << "\t";
    fileID fid("test");
//
    D->addFile(file, fid, prSubset);
    D->print("Blocks", prSubset.get(), prSubset.getSize());
    D->finalize();

    
////
//    __gnu_cxx::hash_map<uint32_t, uint32_t*> map;
//
//    uint32_t* dInteger = new uint32_t[10]();
//    map[0] = dInteger;
//    map[1] = dInteger;
//    const unsigned int pointer = 1;
//    for(int i = 0; map[i] != NULL; i++)
//        cout << map[i];
//    
//    struct test{
//        uint32_t test1;
//        unsigned char* test2;
//    };
//    
//    test test1;
//    test1.test1 = 22;
//    test1.test2 = new unsigned char[4]();
//    
//    vector<test> v;
//    v.push_back(test1);
//    
//    for(int i = 0; i < v.size(); i++)
//        cout << &v.data()[i];

    
//    map[0] = dataS;
    
//    cout << map[0];
//    cout << map[1];
    
//    int chartoget = 32;
//
////
//    byte datat[TOTAL_BLOCKS][BLOCK_SIZE];
//    // insert code here...
//    //DataBlock* block = new DataBlock();
//    //DataBlock block;
//    int num = 5;
//    DataBlock** blocks = new DataBlock*[num];
//    for(int i = 0; i < num; i++){
//        blocks[i] = new DataBlock(i, *new fileID(), data[i], chartoget);
//        cout << blocks[i]->getDecrypted() << endl;
//    }
////
////    for(int i = 0; i < num; i++){
//        blocks[i]->add(i, 1025, data[i], chartoget);
//        unsigned char* d = blocks[i]->get();
//        print("decrypted", d);
//    }
//
//    byte* key = new byte[16]();
//    byte* iv = new byte[16]();
//    
////    clock_t t1,t2;
////    t1=clock();
////    AES* ci = new AES();
////    for(int j = 0; j < TOTAL_BLOCKS; j++)
////        ci->ENC(data[1], BLOCK_SIZE, key, iv);
////    t2=clock();
////    cout << "Continous Encryption took " << ((float)t2-(float)t1) / CLOCKS_PER_SEC << endl;
////    
////    t1=clock();
////    D* DD = new D();
////    t2=clock();
////    cout << "Initilization took " << ((float)t2-(float)t1) / CLOCKS_PER_SEC << endl;
////    
////    t1=clock();
////    for(int i = 0; i < TOTAL_BLOCKS; i++){
////        DD->add(i, 1025, data[0], chartoget);
////    }
////    t2=clock();
////    cout << "Adding blocks took " << ((float)t2-(float)t1) / CLOCKS_PER_SEC << endl;
////    
////    t1=clock();
////    DD->encryptUnusedBlocks();
////    t2=clock();
////    cout << "Encryption took " << ((float)t2-(float)t1) /CLOCKS_PER_SEC << endl;
////
////    t1=clock();
////    for(int i = 0; i < TOTAL_BLOCKS; i++){
////        unsigned char * d = DD->get(i);
//////        print("Decryted D Blocks", d);
////    }
////    t2=clock();
////    cout << "Retreival and decryption took " << ((float)t2-(float)t1) / CLOCKS_PER_SEC<< endl;
////
//    
//    
//    
//    
//    
////    for(int i = 0; i < num; i++){
////        blocks[i]->set(1025, data[i], chartoget);
////        unsigned char* d = blocks[i]->get();
////        print("decrypted", d);
////    }
//    
////    Botan::LibraryInitializer init;
//
////    FFX ffx;
////    ffx.keyGen("hello");
////    cout << "FFX encryption is " + ffx.ENC(1,1);
//    
//    
//    
//    
////    for(int i = 0; i < chartoget; i++){
////        cout << d[i] << ":";
////    }
////    for(int i = chartoget; i < BLOCKSIZE; i++){
////        cout << (int) d[i] << ":";
////    }
////    
////   cout << (int)d[BLOCKSIZE] << ":" << *(uint32_t *)(d+BLOCKSIZE+1);
//    
////    AES* cipher = new AES();
////    
////    byte* key = cipher->keyGen();
////    byte* iv = cipher->keyGen();
//////    byte* iv = new byte[16];
//////    byte* key = new byte[16];
//////    
////    byte* ciphertext = cipher->ENC(data, 32, key, iv );
////    cout << "Ciphertext is " << ciphertext << endl;
////    byte* plaintext = cipher->DEC(ciphertext, 32, key, iv);
////    
////    cout << "Recovered plaintext is " << plaintext << endl;
//    
//    
    
    
    return 0;
    
}





