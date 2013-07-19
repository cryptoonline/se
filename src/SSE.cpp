//
// Searchable Encryption
// SSE.cpp
//

#include "SSE.h"

byte SSE::docHashKey[SSE_DIGEST_SIZE] = {0};
bool SSE::hashKeyGenerated = false;

SSE::SSE(){
	// TODO: construct and initialize BStore object
	if(!hashKeyGenerated)
		setupKey();	
}

SSE::~SSE(){
}

void SSE::setupKey(){
	Key hmacKey(SSE_HMAC_KEYFILE, HMAC_KEY_SIZE);
	hmacKey.get(docHashKey);
	hashKeyGenerated = true;
}

void SSE::indexgen(string directoryPath){
	
	genPlainIndex(directoryPath);

	for(unordered_map<string, unordered_set<docid_t> >::iterator itmap = map.begin(); itmap != map.end(); ++itmap){
		const string& keyword = itmap->first; //filename in BStore
		unordered_set<docid_t>& set = itmap->second;
		
		byte docList[set.size()*sizeof(docid_t)];
		uint32_t counter = 0;
		for(unordered_set<docid_t>::iterator itset = set.begin(); itset != set.end(); ++itset){
			docid_t documentID = *itset;
			memcpy(&docList[counter*sizeof(docid_t)], static_cast<byte*>(static_cast<void*>(&documentID)), sizeof(docid_t));
			counter++;
		}
//		printhex(docList, set.size()*sizeof(docid_t), "DOC LIST");
		store.add(keyword, docList, set.size()*sizeof(docid_t));
	}
	store.finalize();
}

docid_t SSE::getDocNameHash(string& docname){
	HashMAC hash;
	hash.setKey(docHashKey);
	byte hashMacBytes[SSE_DIGEST_SIZE];
	hash.doFinal(docname, hashMacBytes);
	return *(uint64_t*)hashMacBytes;
}

void SSE::genPlainIndex(string directoryPath) {
	cout << "[PATH] " << directoryPath << endl;

	/* OPTIONAL: Print progress */
	int fileSum = 0, fileCount = 0;
	for(boost::filesystem::recursive_directory_iterator end, dir(directoryPath); dir != end; ++dir) {
		if(boost::filesystem::is_regular(dir->status())) {
			fileSum++;
		}
	}

	for(boost::filesystem::recursive_directory_iterator end, dir(directoryPath); dir != end; ++dir) {
//		string fileName = boost::filesystem::canonical(dir->path()).string();
		string fileName = dir->path().string();
		if(boost::filesystem::is_regular(dir->status())) {
			cout << "[FILE] " << fileName << endl;
			fileCount++;
			docid_t hash = getDocNameHash(fileName);
			hash &= 0x7FFFFFFFFFFFFFFFL;
			cout << "Hash is " << hash << "." << endl;
			ifstream input(fileName.c_str());
			string getcontent;
			boost::tokenizer<> tok(getcontent);
			while(getline(input, getcontent)) {
				tok.assign(getcontent.begin(), getcontent.end());
				for(boost::tokenizer<>::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
					string keyword(*beg);
					// OPTIONAL: convert keywords to lower case
					std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
//					cout << "[KWRD] " << keyword << endl;
					// Possible optimization: memorize hash of fileName
					// add keyword --> fileName to the map
					map[keyword].insert(hash);
				}
			}
			input.close();
		} else {
			// it's a directory
			cout << "[DIR] [" << (int)((double)fileCount/(double)fileSum*100) << "%] " << fileName << endl;
		}
	}
	cout << "[DONE] Number of keywords: " << map.size() << std::endl;

	// OPTIONAL: Estimate map size
	uint64_t mapSize = 0;
	for(unordered_map<string, unordered_set<docid_t> >::iterator itmap = map.begin(); itmap != map.end(); ++itmap) {
		const string & keyword = itmap->first;
		mapSize += keyword.size();
		unordered_set<docid_t> & set = itmap->second;
		for(unordered_set<docid_t>::iterator itset = set.begin(); itset != set.end(); ++itset) {
			docid_t documentId = *itset;
			mapSize += 8;
		}
	}
	cout << "[DONE] Estimated map size: " << mapSize << endl;

	// TODO: copy all documents to DocumentStore, rename each file to its docId
}

void SSE::remove(string document){
	
	// TODO: delete docNameHash(document) from DocumentStore
}

void SSE::add(string path){
	// TODO
	
}

bool SSE::search(string keyword, vector<docid_t>& docIDs){
	OnlineSession session;
	byte* docIDsBytes;
	b_index_t size = session.read(keyword, docIDsBytes);

	if(size == 0)
		return false;
	
	for(b_index_t i = 0; i < size/sizeof(docid_t); i++)
		docIDs.push_back(*(docid_t*)(&docIDsBytes[i*sizeof(docid_t)]));
	
	if(docIDsBytes)
		delete[] docIDsBytes;

	return true;
	// TODO: check for file with filename docNameHash(document) in
}
