//
// optT.cpp
// BlindStorage
//

#include "optT.h"


optT::optT(){
	Key key(OT_KEYFILE, HMAC_KEY_SIZE);
	byte keyBytes[HMAC_KEY_SIZE];
	key.get(keyBytes);
	hash.setKey(keyBytes);
}

optT::~optT(){
}

void optT::addFile(string filename, size_t filesize, PRSubset prSubset){
	byte mac[SHA_BLOCK_SIZE];
	hash.doFinal(filename, mac);

	uint64_t pairkey = *(uint64_t*)mac;

	T.insert(std::make_pair<uint64_t, PRSubset>(pairkey, prSubset));
}

void optT::writeToDisk(){
	ofstream file(OT_FILE, std::ios::out | std::ios::binary);
	char buffer[sizeof(uint64_t)+sizeof(prSubsetSize_t)+sizeof(prSubsetSeed_t)];
	cout << "Writing T to Local disk. Please wait!" << endl;
	for(unordered_map<uint64_t, PRSubset>::iterator iter=T.begin(); iter!=T.end(); ++iter){
		uint64_t key = iter->first;
		PRSubset prSubset = iter->second;

		prSubsetSize_t size = prSubset.getSize();
		prSubsetSeed_t seed = prSubset.getSeed();

		file.write(reinterpret_cast<char*>(&key), sizeof(uint64_t));
		file.write(reinterpret_cast<char*>(&size), sizeof(prSubsetSize_t));
		file.write(reinterpret_cast<char*>(&seed), sizeof(prSubsetSeed_t));
	}

	file.close();
	cout << "T written to local disk." << endl;

	/*
	char readBytes[16];
	ifstream in(OT_FILE, std::ios::in | std::ios::binary);
	in.seekg(0);
	in.read(readBytes, 16);

	printhex(readBytes, 16, "Bytes");
	cout << "Key is " << *(uint64_t*)(readBytes) << endl;
	cout << "Size is " << *(prSubsetSize_t*)(readBytes+sizeof(uint64_t)) << endl;
	cout << "Seed is " << *(prSubsetSeed_t*)(readBytes+sizeof(uint64_t)+sizeof(prSubsetSize_t)) << endl;
	*/

	}
