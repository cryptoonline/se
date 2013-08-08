//
// Searchable Encryption
// SSE.cpp
//

#include "SSE.h"

SSE::SSE(){
	setupKey();
//	store = new BStore();
}

SSE::~SSE(){
}

void SSE::setupKey(){
	Key hmacKey(SSE_HMAC_KEYFILE, HMAC_KEY_SIZE);
	hmacKey.get(docHashKey);
	hashMac.setKey(docHashKey);
}

void SSE::indexgen(string directoryPath, double& execTime){

	BStore store;
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
		clock_t startTime = clock();
		store.add(boost::lexical_cast<string>(0)+keyword, docList, set.size()*sizeof(docid_t));
		execTime  += (double)(clock() - startTime)/(double)CLOCKS_PER_SEC;
	}
	store.finalize(execTime);
}

uint64_t SSE::getDocNameHash(string docname){
	byte hashMacBytes[SSE_DIGEST_SIZE];
	hashMac.doFinal(docname, hashMacBytes);
	return *(uint64_t*)hashMacBytes;
}

void SSE::genPlainIndex(string directoryPath) {
	cout << "[PATH] " << directoryPath << endl;

	/* OPTIONAL: Print progress */
	int fileSum = 0, fileCount = -1;
	for(boost::filesystem::recursive_directory_iterator end, dir(directoryPath); dir != end; ++dir) {
		if(boost::filesystem::is_regular(dir->status())) {
			fileSum++;
		}
	}

	double SKETime = 0;
	double filestoreTime = 0;
	double plainIndexTime = 0;

	clock_t startTime = clock();
	for(boost::filesystem::recursive_directory_iterator end, dir(directoryPath); dir != end; ++dir) {
//		string fileName = boost::filesystem::canonical(dir->path()).string();
		
		string fileName = dir->path().string();


		if((dir->path().filename()).compare(".DS_Store") == 0){
//			cout << "Ignoring file " << dir->path().filename() << endl;
			continue;
		}

		if(boost::filesystem::is_regular(dir->status())) {
			fileCount++;
	//		cout << "[FILE] " << fileName << "--->" << fileCount << endl;
			uint64_t docID = getDocNameHash(boost::lexical_cast<string>(fileCount));
			
			CLEAR_BIT(docID, 0);
			
			/* Put file contents, FileStore is responsible for enryption and decryption of data files*/

			clock_t startTime = clock();
			double cryptoduration = 0;
			storefile(fileName, docID, cryptoduration);
			SKETime += cryptoduration;
			filestoreTime += (double)(clock()-startTime)/(double)CLOCKS_PER_SEC;

			ifstream input(fileName.c_str());

//			char charsToRemove[] = "~!@#$%^&*()_+-=[]{};:'\"\\|?/<>,.";

			boost::char_separator<char> sep(" ~!#$%^&*()+=[]{};:'\"\\|?/<>,");
			string getcontent;
			boost::tokenizer<boost::char_separator<char> > tok(getcontent);
			while(getline(input, getcontent)) {
//				cout << getcontent << endl;
//				for(unsigned int i = 0; i < strlen(charsToRemove); i++)
//					getcontent.erase(std::remove(getcontent.begin(), getcontent.end(), charsToRemove[i]), getcontent.end());
//				cout << getcontent << endl;
				tok.assign(getcontent.begin(), getcontent.end());
				for(boost::tokenizer<boost::char_separator<char> >::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
					string keyword(*beg);
					// OPTIONAL: convert keywords to lower case
					std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
//					cout << "[KWRD] " << keyword << endl;
					// Possible optimization: memorize docID of fileName
					// add keyword --> fileName to the map
//					map[keyword].insert(docID);
					map[keyword].insert(fileCount);
				}
			}
			input.close();
		}
//		else {
//			// it's a directory
//			cout << "[DIR] [" << (int)((double)fileCount/(double)fileSum*100) << "%] " << fileName << endl;
//		}
	}
	cout << "[DONE] Number of keywords: " << map.size() << std::endl;

	plainIndexTime = (double)(clock()-startTime)/(double)CLOCKS_PER_SEC - filestoreTime;
	cout << "Plain index gen took " << plainIndexTime << " seconds." << endl;
	cout << "SKE took " << SKETime << " seconds." << endl;

	// OPTIONAL: Estimate map size
	uint64_t mapSize = 0;
	uint64_t fileKeywordPairs = 0;
	for(unordered_map<string, unordered_set<docid_t> , stringhash>::iterator itmap = map.begin(); itmap != map.end(); ++itmap) {
		const string & keyword = itmap->first;
//		mapSize += keyword.size();
		unordered_set<docid_t> & set = itmap->second;
		for(unordered_set<docid_t>::iterator itset = set.begin(); itset != set.end(); ++itset) {
			docid_t documentId = *itset;
			mapSize += 4;
			fileKeywordPairs++;
		}
	}
	cout << "[DONE] Estimated map size: " << mapSize << endl;
	cout << "File keyword pairs are: " << fileKeywordPairs << endl;
	// TODO: copy all documents to DocumentStore, rename each file to its docId
}

void SSE::remove(docid_t docName, double& duration){
//	docName = "/Users/naveed/BStore/datasets/testdir/" + docName + ".";
	
	clock_t startTime = clock();
	uint64_t docHash = getDocNameHash(boost::lexical_cast<string>(docName));
	uint64_t docID0 = docHash;	CLEAR_BIT(docID0, 0);
	uint64_t docID1 = docHash;	SET_BIT(docID1, 0);

	duration += (double)(clock()-startTime)/(double)CLOCKS_PER_SEC;

	OnlineSession session;
	session.resetDiskAccessTime();

	clock_t ignoredStartTime = clock();
	if(fstore.isFilePresent(boost::lexical_cast<string>(docID0))){
		fstore.remove(boost::lexical_cast<string>(docID0));
//		duration -= (double)(clock()-ignoredStartTime)/(double)CLOCKS_PER_SEC;
		/* Entries from Index will be delete using lazy delete*/
	}
	else if (fstore.isFilePresent(boost::lexical_cast<string>(docID1))){
//	else if (fstore.isFilePresent(boost::lexical_cast<string>(docID))){
		byte* doc;
		double SKETime = 0;
//		size_t size = fstore.get("/Users/naveed/BStore/datasets/test4MB/allen-p/straw/8.", doc, SKETime);
		size_t size = fstore.get(boost::lexical_cast<string>(docID1), doc, SKETime);
		cout << "SKE took " << SKETime << " seconds." << endl;

		unordered_set<string, stringhash> keywords;
		getKeywords(doc, size, keywords);

		duration -= (double)(clock()-ignoredStartTime)/(double)CLOCKS_PER_SEC;
		
		for(unordered_set<string, stringhash>::iterator it = keywords.begin(); it != keywords.end(); ++it){
			string keyword = boost::lexical_cast<string>(1) + *it;
			OnlineSession session;
			byte* docIDs = NULL;
			size_t size = session.updateRead(keyword, docIDs, -sizeof(docid_t));
//			cout << "Updating keyword " << keyword << endl;
//			cout << "Number of files containing keyword \"" << keyword << "\" are " << size << endl << " " << size - sizeof(docid_t) << endl;
//			uint32_t docIDtoRemove = findDocID(docIDs, size, docID1);
			uint32_t docIDtoRemove = findDocID(docIDs, size, docName);
			if(docIDtoRemove == -1){
				std::cerr << "File present in filestore but is not present in BStore." << endl;
				std::cerr << "This is probably due to the files stored from previoius runs. Try again after deleting files from previous runs." << endl;
				exit(1);
			}
			deleteDocID(docIDs, size, docIDtoRemove);
//			cout << "DocID to remove " << docIDtoRemove << endl;
			session.updateWrite(keyword, docIDs, size);
			delete[] docIDs;
			docIDs = NULL;
		}
	
		delete[] doc;

		duration += (double)(clock()-startTime)/(double)CLOCKS_PER_SEC - session.getDiskAccessTime();

		cout << "Disk operations took " << session.getDiskAccessTime() << " seconds." << endl;
		fstore.remove(boost::lexical_cast<string>(docID1));

	}
	else{
		cout << "File not found!" << endl;
	}
	// TODO: delete docNameHash(document) from DocumentStore
}

void SSE::add(docid_t docName, string docFileName, double& duration){
	clock_t startTime = clock();
	uint64_t docID = getDocNameHash(boost::lexical_cast<string>(docName));
	SET_BIT(docID, 0);

	byte* doc;

	clock_t ignoredStartTime = clock();
	size_t size = fstore.getNew(docFileName, doc);

	unordered_set<string, stringhash> keywords;
	getKeywords(doc, size, keywords);
	
	duration -= (double)(clock() - ignoredStartTime)/(double)CLOCKS_PER_SEC;

	remove(docName, duration);
	
	OnlineSession session;
	session.resetDiskAccessTime();

//	cout << "Adding keywords " << endl;
	for(unordered_set<string, stringhash>::iterator it = keywords.begin(); it != keywords.end(); ++it){
		string keyword = boost::lexical_cast<string>(1) + *it;
		
		OnlineSession session;
		
		byte* docIDs;
		size_t size = session.updateRead(keyword, docIDs, sizeof(docid_t));
//		cout << "Size is " << size << endl;
//		addDocID(docIDs, size, docID);
//		printhex(docIDs, size, "BEFORE");
		byte updatedDocIDs[size+sizeof(docid_t)];
		memcpy(updatedDocIDs, docIDs, size);
		memcpy(&updatedDocIDs[size], static_cast<byte*>(static_cast<void*>(&docName)), sizeof(docid_t));
//		printhex(updatedDocIDs, size+sizeof(docid_t), "AFTER");
		session.updateWrite(keyword, updatedDocIDs, size + sizeof(docid_t));
		delete[] docIDs;
	}

//	cout << "Disk operations took " << session.getDiskAccessTime() << " seconds." << endl;
	
	duration += (double)(clock()-startTime)/(double)CLOCKS_PER_SEC - session.getDiskAccessTime();

	cout << "Disk Access took " << session.getDiskAccessTime() << " seconds." << endl;
	
	double SKETime = 0;
	fstore.put(boost::lexical_cast<string>(docID), doc, size, SKETime);
	cout << "SKE took write" << SKETime << " seconds." << endl;
	delete[] doc;
}

bool SSE::search(string keyword, vector<docid_t>& docIDs, double& duration){
	clock_t startTime = clock();
	OnlineSession session;
	session.resetDiskAccessTime();

	bool docsFound = false;

	bool docsFoundInitial = retrieveIndex0(boost::lexical_cast<string>(0)+keyword, docIDs);
	
	bool docsFoundUpdate = retrieveIndex1(boost::lexical_cast<string>(1)+keyword, docIDs);

	duration = (double)(clock()-startTime)/(double)CLOCKS_PER_SEC - session.getDiskAccessTime();
	cout << "Disk Access took " << session.getDiskAccessTime() << " seconds." << endl;
	
	if(docsFoundInitial || docsFoundUpdate)
		return true;
}

bool SSE::retrieveIndex0(string keyword, vector<docid_t>& docIDs){
	OnlineSession session;
	byte* docIDsBytes;

	size_t size = session.updateRead(keyword, docIDsBytes, 0);

	if(size == 0)
		return false;

	byte updatedDocIDsBytes[size];
	memset(updatedDocIDsBytes, 0, size);

	int32_t j = 0;
	for(int32_t i = 0; i < size/sizeof(docid_t); i++){
		docid_t docID = *(docid_t*)(&docIDsBytes[i*sizeof(docid_t)]);
		uint64_t docIDonDisk = getDocNameHash(boost::lexical_cast<string>(docID));
		CLEAR_BIT(docIDonDisk, 0);
		if(fstore.isFilePresent(boost::lexical_cast<string>(docIDonDisk))){
			docIDs.push_back(docID);
			memcpy(&updatedDocIDsBytes[j*sizeof(docid_t)], &docIDsBytes[i*sizeof(docid_t)], sizeof(docid_t));
			j++;
		}
	}

	session.updateWrite(keyword, updatedDocIDsBytes, size);
	delete[] docIDsBytes;

	return true;
}

bool SSE::retrieveIndex1(string keyword, vector<docid_t>& docIDs){
	OnlineSession session;
	session.resetDiskAccessTime();
	byte* docIDsBytes;
	size_t size = session.read(keyword, docIDsBytes);

	if(size == 0)
		return false;

	for(int32_t i = 0; i < size/sizeof(docid_t); i++)
		docIDs.push_back(*(docid_t*)(&docIDsBytes[i*sizeof(docid_t)]));

	delete[] docIDsBytes;

	return true;
}

void SSE::getKeywords(byte docBytes[], size_t size, unordered_set<string, stringhash>& keywords){
	clock_t startTime = clock();
	string content(reinterpret_cast<char*>(docBytes), size);
//	cout << "Content is " << content << endl;
	boost::tokenizer<> tok(content);

	tok.assign(content.begin(), content.end());
	for(boost::tokenizer<>::iterator it = tok.begin(); it != tok.end(); ++it){
		string keyword(*it);
		std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
		keywords.insert(keyword);
	}
	cout << "Keyword generation took " << ((double)(clock() - startTime)/(double)CLOCKS_PER_SEC) << endl;;
}

int32_t SSE::findDocID(byte* docIDs, size_t size, docid_t docID){
	for(int32_t i = 0; i < size/sizeof(docid_t); i++){
		if(*(docid_t*)(&docIDs[i*sizeof(docid_t)]) == docID)
			return i;
	}
	return -1;
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
	uint32_t i = docIDIndex;
	for(; i < size-sizeof(docid_t); i++)
		docIDs[i] = docIDs[i+sizeof(docid_t)];
	memset(&docIDs[i], 0, sizeof(docid_t));
}

void SSE::storefile(string filename, uint64_t docID, double& cryptoduration){
	size_t filesize = readFileSize(filename);
	byte* docBytes = new byte[filesize];
	readFile(filename, docBytes, filesize);
	fstore.put(boost::lexical_cast<string>(docID), docBytes, filesize, cryptoduration);
	delete[] docBytes;
}
