//
//  main.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "DataBlock.h"
#include "AES.h"
#include "FFX.h"

void print(string tag, unsigned char* value){
    cout << "<" << tag << ">" << value << "</" << tag << ">" << endl;
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
    // insert code here...
    //DataBlock* block = new DataBlock();
    //DataBlock block;
    int num = 2;
    DataBlock** blocks = new DataBlock*[num];
    for(int i = 0; i < num; i++){
        blocks[i] = new DataBlock();
        
    }
    
    int chartoget = 32;
    for(int i = 0; i < num; i++){
        blocks[i]->set(1025, data[i], chartoget);
        unsigned char* d = blocks[i]->get();
        print("decrypted", d);
    }
    
//    for(int i = 0; i < num; i++){
//        blocks[i]->set(1025, data[i], chartoget);
//        unsigned char* d = blocks[i]->get();
//        print("decrypted", d);
//    }
    
//    Botan::LibraryInitializer init;

//    FFX ffx;
//    ffx.keyGen("hello");
//    cout << "FFX encryption is " + ffx.ENC(1,1);
    
    
    
    
//    for(int i = 0; i < chartoget; i++){
//        cout << d[i] << ":";
//    }
//    for(int i = chartoget; i < BLOCKSIZE; i++){
//        cout << (int) d[i] << ":";
//    }
//    
//   cout << (int)d[BLOCKSIZE] << ":" << *(uint32_t *)(d+BLOCKSIZE+1);
    
//    AES* cipher = new AES();
//    
//    byte* key = cipher->keyGen();
//    byte* iv = cipher->keyGen();
////    byte* iv = new byte[16];
////    byte* key = new byte[16];
////    
//    byte* ciphertext = cipher->ENC(data, 32, key, iv );
//    cout << "Ciphertext is " << ciphertext << endl;
//    byte* plaintext = cipher->DEC(ciphertext, 32, key, iv);
//    
//    cout << "Recovered plaintext is " << plaintext << endl;
    
    
    
    
    return 0;
    
}



