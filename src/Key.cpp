//
// Key.cpp
// BlindStorage
//

#include "Key.h"

Key::Key(string filename, int size): key(size){
	this->filename = filename;
	this->keySize = size;
//	key = new char[keySize];
	setup();
}

Key::~Key(){
//	delete[] key;
}

void Key::get(byte key[]){
//	memcpy(key, this->key, keySize);
	std::copy(this->key.begin(), this->key.end(), key);
}

void Key::save(){
	keyfileWrite.open(filename.c_str());
//	keyfileWrite.seekg(std::ios::beg);
	char key[keySize];
	std::copy(this->key.begin(), this->key.end(), key);
	keyfileWrite.write(key, keySize);
	if(!keyfileWrite.good())
		cerr << "File is not good " << __LINE__;
}


void Key::load(){
	char key[keySize];
	if(!keyfileRead.good()){
		keyfileRead.open(filename.c_str());
		cerr << "File is not good " << __LINE__;
		exit(1);
	}
	keyfileRead.seekg(std::ios::beg);
	keyfileRead.read(key, keySize);	
	this->key.insert(this->key.begin(), &key[0], &key[keySize]);
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
	byte key[keySize];
	CryptoPP::AutoSeededRandomPool prng;
	prng.GenerateBlock(key, keySize);
	this->key.insert(this->key.begin(), &key[0], &key[keySize]);
}

bool Key::isKeyPresentOnDisk(){
	keyfileRead.open(filename.c_str());
	if(keyfileRead.good())
		return true;
	else
		return false;
}
