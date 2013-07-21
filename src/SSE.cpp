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

	for(unordered_map<string, unordered_set<docid_t>, stringhash >::iterator itmap = map.begin(); itmap != map.end(); ++itmap){
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
//			cout << "[FILE] " << fileName << endl;
			fileCount++;
			docid_t docID = getDocNameHash(fileName);

			docID &= 0x7FFFFFFFFFFFFFFFL;
//			cout << "Hash is " << docID << "." << endl;
			
			/* Put file contents, FileStore is responsible for enryption and decryption of data files*/
			storefile(fileName, docID);

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
					// Possible optimization: memorize docID of fileName
					// add keyword --> fileName to the map
					map[keyword].insert(docID);
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
	for(unordered_map<string, unordered_set<docid_t> , stringhash>::iterator itmap = map.begin(); itmap != map.end(); ++itmap) {
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

void SSE::remove(string docName){
//	docName = "/Users/naveed/BStore/datasets/testdir/" + docName + ".";
	docid_t docID = getDocNameHash(docName);

	cout << "File to remove is " << boost::lexical_cast<string>(docID & 0x7FFFFFFFFFFFFFFFL) << endl; 
	if(fstore.isFilePresent(boost::lexical_cast<string>(docID & 0x7FFFFFFFFFFFFFFFL))){
		fstore.remove(boost::lexical_cast<string>(docID & 0x7FFFFFFFFFFFFFFFL));
		/* Entries from Index will be delete using lazy delete*/
	}
	else if (fstore.isFilePresent(boost::lexical_cast<string>(docID | 0x8000000000000000L))){
//	else if (fstore.isFilePresent(boost::lexical_cast<string>(docID))){
		byte* doc;
		size_t size = fstore.get(docName, doc);
		cout << "File size is " << size << endl;
		printhex(doc, size, "DOC");

		unordered_set<string, stringhash> keywords;
		getKeywords(doc, size, keywords);

		for(unordered_set<string, stringhash>::iterator it = keywords.begin(); it != keywords.end(); ++it){
			string keyword = *it;
			OnlineSession session;
			byte* docIDs;
			size_t size = session.updateRead(keyword, docIDs, 0);
			cout << "Updating keyword " << keyword << endl;
			cout << "Number of files containing keyword \"" << keyword << "\" are " << size << endl << " " << size - sizeof(docid_t) << endl;
			uint32_t docIDtoRemove = findDocID(docIDs, size, docID);
			printhex(docIDs, size, "DocIDs BEFORE");
			deleteDocID(docIDs, size, docIDtoRemove);
			printhex(docIDs, size, "DocIDs AFTER");
			cout << "DocID to remove " << docIDtoRemove << endl;
			session.updateWrite(keyword, docIDs, size);
			delete[] docIDs;
		}
		
		fstore.remove(boost::lexical_cast<string>(docID | 0x8000000000000000L));

		delete[] doc;
	}
	else{
		cout << "File not found!" << endl;
	}
	// TODO: delete docNameHash(document) from DocumentStore
}

void SSE::add(string docName){
	docid_t docID = getDocNameHash(docName) | 0x8000000000000000L;
	byte* doc;
	size_t size = fstore.get(docName, doc);

	cout << "Adding file " << docName << endl;
	printchars(doc, size, "File being added");

	unordered_set<string, stringhash> keywords;
	getKeywords(doc, size, keywords);

	remove(docName);

	cout << "Adding keywords " << endl;
	for(unordered_set<string, stringhash>::iterator it = keywords.begin(); it != keywords.end(); ++it){
		string keyword = *it;

		cout << "Add " << keyword << endl;
		OnlineSession session;
		byte* docIDs;
		size_t size = session.updateRead(keyword, docIDs, sizeof(docid_t));
	//	addDocID(docIDs, size, docID);
		byte updatedDocIDs[size+sizeof(docid_t)];
		memcpy(updatedDocIDs, docIDs, size);
		memcpy(&updatedDocIDs[size], static_cast<byte*>(static_cast<void*>(&docID)), sizeof(docid_t));
		session.updateWrite(keyword, updatedDocIDs, size + sizeof(docid_t));
//		delete[] docIDs;
	}

	fstore.put(boost::lexical_cast<string>(docID), doc, size);

	delete[] doc;
}

bool SSE::search(string keyword, vector<docid_t>& docIDs){
	OnlineSession session;
	byte* docIDsBytes;
	b_index_t size = session.read(keyword, docIDsBytes);

	if(size == 0)
		return false;
	
	for(b_index_t i = 0; i < size/sizeof(docid_t); i++)
		docIDs.push_back(*(docid_t*)(&docIDsBytes[i*sizeof(docid_t)]));

	cout << "No. of documents with keyword " << docIDs.size() << endl;

//	if(docIDsBytes)
//		delete[] docIDsBytes;

	return true;
	// TODO: check for file with filename docNameHash(document) in
}

void SSE::getKeywords(byte docBytes[], size_t size, unordered_set<string, stringhash>& keywords){
	string content(reinterpret_cast<char*>(docBytes), size);
	cout << "Content is " << content << endl;
	boost::tokenizer<> tok(content);

	tok.assign(content.begin(), content.end());
	for(boost::tokenizer<>::iterator it = tok.begin(); it != tok.end(); ++it){
		string keyword(*it);
		std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
		keywords.insert(keyword);
	}
}

uint32_t SSE::findDocID(byte* docIDs, size_t size, docid_t docID){
	for(int32_t i = 0; i < size/sizeof(docid_t); i++){
		if(*(docid_t*)(&docIDs[i*sizeof(docid_t)]) == docID)
			return i;
	}
	return 0;
}

void SSE::addDocID(byte*& docIDs, size_t size, docid_t docID){
	byte* updatedDocIDs = new byte[size+sizeof(docid_t)];
	memcpy(updatedDocIDs, docIDs, size);
	memcpy(&updatedDocIDs[size], static_cast<byte*>(static_cast<void*>(&docID)), sizeof(docid_t));
//	delete[] docIDs;
	docIDs = updatedDocIDs;
}

void SSE::deleteDocID(byte* docIDs, size_t size, uint32_t docIDIndex){
//	std::copy(docIDs + docIDIndex, docIDs + size, docIDs + docIDIndex - sizeof(docid_t));
	int32_t i = docIDIndex;
	for(; i < size-sizeof(docid_t); i++)
		docIDs[i] = docIDs[i+sizeof(docid_t)];
	memset(&docIDs[i], 0, sizeof(docid_t));
}

void SSE::storefile(string filename, docid_t docID){
	size_t filesize = readFileSize(filename);
	byte* docBytes = new byte[filesize];
	readFile(filename, docBytes, filesize);
	fstore.put(boost::lexical_cast<string>(docID), docBytes, filesize);
	delete[] docBytes;
}
