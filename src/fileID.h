//
//  fileID.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef __BlindStorage__fileID__
#define __BlindStorage__fileID__

#include <iostream>

#include "SHA256bit.h"
#include "FFX.h"

using namespace std;

class fileID {
private:
    unsigned char* higherID;
    unsigned char* lowerID; /// It will be of 28 bytes.
    unsigned char* ID; ///It will be hashedID but with first 4 bytes FFX
    
    string filename;
    
//    static auto_ptr<const unsigned char> hashKey;
//    static auto_ptr<const unsigned char> prpKey;
    const unsigned char* hashKey;
    const unsigned char* prpKey;
    void compute();
    void parse();
    
    FFX ffx;
    unsigned char* PRP(unsigned char* bytes, int32_t size);
    
    SHA256bit sha;
    unsigned char* hmac(string filename);
    
public:
    void initialize();
    fileID();
    fileID(string filename);
    fileID(unsigned char* ID);
    fileID(unsigned char* higherID, bool criID);
    ~fileID();
    void generateKeys();
    unsigned char* get();
    const unsigned char* getLowerID();
    uint32_t getPRPofHigherID();
    void makeCRIID();
};

#endif /* defined(__BlindStorage__fileID__) */
