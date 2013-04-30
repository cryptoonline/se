//
//  fileID.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/21/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "fileID.h"


void fileID::initialize(){
    ID = new unsigned char[32]();
    higherID = new unsigned char[4]();
    lowerID = new unsigned char[28]();
    generateKeys();
    
}

fileID::fileID(){
    initialize();
}

fileID::fileID(string filename){
    initialize();
        this->filename = filename;
    compute();
    if(getPRPofHigherID() == 0){
        cerr << "Invalid higher fileID, 0 file id is reserved for empty file." << endl;
    }
}

fileID::fileID(unsigned char* ID){
    initialize();
	delete[] ID;
    this->ID = ID;
    parse();
}

fileID::fileID(unsigned char* higherID, bool criFile){
    initialize();
    this->higherID = higherID;
}

fileID::~fileID(){
//    delete[] hashKey;
//    delete[] prpKey;
}

unsigned char* fileID::get(){
    return ID;
}

uint32_t fileID::getPRPofHigherID(){
    if(higherID != NULL){
        return *(uint32_t*)higherID;
    } else{
        cerr << "Higher ID is NULL pointer." << endl;
        return 0;
    }
}

const unsigned char* fileID::getLowerID(){
    return lowerID;
}

unsigned char* fileID::hmac(string filename){
    return sha.doFinal(filename);
}

unsigned char* fileID::PRP(unsigned char* bytes, int32_t size){
    return ffx.encrypt(bytes);
}

void fileID::compute(){
    unsigned char* hashedID = hmac(filename);
    
    //lowerID = &hashedID[4];
    //higherID = PRP(hashedID, 4);
    memcpy(lowerID, hashedID+4, 28);
    memcpy(higherID, PRP(hashedID, 4), 4);
    
    memcpy(ID, higherID, 4);
    memcpy(ID+4, lowerID, 28);
}

void fileID::parse(){
    higherID = ID;
    lowerID = ID + 4;
}

void fileID::makeCRIID(){
    delete[] lowerID;
    lowerID = new unsigned char[28](); /// Zero it.
}

void fileID::generateKeys(){
//    auto_ptr<const unsigned char> hashKey(sha.keyGen());
//    this->hashKey = hashKey;
//    auto_ptr<const unsigned char> prpKey(ffx.keyGen());
//    this->prpKey = prpKey;
//    hashKey.reset(sha.keyGen());
//    prpKey.reset(ffx.keyGen());
    
    hashKey = sha.keyGen();
//    cout << hashKey;
    prpKey = ffx.keyGen();
}
