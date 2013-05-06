//
// Key.cpp
// BlindStorage
//

#include "Key.h"

Key::Key(string filename){
	this->filename = filename;
	key = new char[KEY_SIZE];
	setup();
}

Key::~Key(){
}

char* Key::get(){
	return key;
}

void Key::save(){
	keyfileWrite.open(filename.c_str());
//	keyfileWrite.seekg(std::ios::beg);
	keyfileWrite.write(key, KEY_SIZE);
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
	keyfileRead.read(key, KEY_SIZE);	
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
	AES cipher;
	key = reinterpret_cast<char*>(cipher.keyGen());
}

bool Key::isKeyPresentOnDisk(){
	keyfileRead.open(filename.c_str());
	if(keyfileRead.good())
		return true;
	else
		return false;
}

