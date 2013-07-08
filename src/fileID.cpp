//
// fileID.cpp
// BlindStorage
//

#include "fileID.h"

bool fileID::keyGenerated = false;

byte fileID::hmacKey[HMAC_KEY_SIZE] = {0};
byte fileID::prfKey[PRF_KEY_SIZE] = {0};

fileID::fileID(){
}

fileID::fileID(string filename) {
	if(!keyGenerated)
		setupKey();

	HashMAC hmac;
	hmac.setKey(hmacKey);

	byte mac[SHA_BLOCK_SIZE];
	hmac.doFinal(filename, mac);

	PRF prf;
	prf.setKey(prfKey);
	higherID = prf.encrypt(mac, BIT_MASK);

	memcpy(ID, static_cast<byte*>(static_cast<void*>(&higherID)), HIGHERFID_SIZE);
	memcpy(ID+HIGHERFID_SIZE, &mac[HIGHERFID_SIZE], LOWERFID_SIZE);
}

fileID::fileID(byte ID[]) {
	memcpy(this->ID, ID, FILEID_SIZE);
	higherID = *(higherfid_t*)(ID);
}

fileID::~fileID() {
}

void fileID::get(byte ID[]) {
	memcpy(ID, this->ID, FILEID_SIZE);
}

void fileID::getLowerID(byte lowerID[]){
	memcpy(lowerID, &ID[HIGHERFID_SIZE], LOWERFID_SIZE);
}

higherfid_t fileID::getHigherID(){
	return *(higherfid_t*)(ID);
}

void fileID::setupKey() {
	Key key0(FID_HMAC_KEYFILE, HMAC_KEY_SIZE);
	key0.get(hmacKey);

	Key key1(FID_PRF_KEYFILE, PRF_KEY_SIZE);
	key1.get(prfKey);

	keyGenerated = true;
}
