//
// Key.cpp
// BlindStorage
//

#include "Key.h"

Key::Key(const char filename[]){
	key = new char[KEY_SIZE];
	keyfile.open(filename);
	setup();
}

Key::~Key(){
	keyfile.close();
	delete[] key;
}

char* Key::get(){
	return key;
}

void Key::save(){
	keyfile.write(key, KEY_SIZE);
}


void Key::load(){
	keyfile.read(key, KEY_SIZE);	
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
	if(keyfile.good())
		return true;
	else
		return false;
}

