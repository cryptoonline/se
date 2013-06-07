//
// Key.cpp
// BlindStorage
//

#include "Key.h"

Key::Key(string filename, int keySize){
	this->filename = filename;
	this->keySize = keySize;
	key = new char[keySize];
	setup();
}

Key::~Key(){
	delete[] key;
}

void Key::get(byte key[]){
	memcpy(key, this->key, keySize);
}

void Key::save(){
	keyfileWrite.open(filename.c_str());
//	keyfileWrite.seekg(std::ios::beg);
	keyfileWrite.write(key, keySize);
	if(!keyfileWrite.good())
		cerr << "File is not good " << __LINE__;
}


void Key::load(){
	if(!keyfileRead.good()){
		keyfileRead.open(filename.c_str());
		cerr << "File is not good " << __LINE__;
		exit(1);
	}
	keyfileRead.seekg(std::ios::beg);
	keyfileRead.read(key, keySize);	
}

void Key::setup(){
	if(!isKeyPresentOnDisk()){
		generate();
		save();
	}
	else{
		load();
	}
		
}

void Key::generate(){
	CryptoPP::AutoSeededRandomPool prng;
	prng.GenerateBlock(reinterpret_cast<byte*>(key), keySize);
}

bool Key::isKeyPresentOnDisk(){
	keyfileRead.open(filename.c_str());
	if(keyfileRead.good())
		return true;
	else
		return false;
}

