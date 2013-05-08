#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "SHA256bit.h"
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>

#include "SSE.h"

using std::cout;
using std::endl;
using std::ifstream;

docid_t SSE::docNameHash(string & doc){
	SHA256bit hash;
	hash.keyGen();
	unsigned char* fileNameHash = hash.doFinal(doc);
	docid_t result = *(uint64_t*)fileNameHash;
	return result;
}

SSE::SSE(){
	// TODO: construct and initialize BStore object
}

void SSE::indexGen(string path)
{
	cout << "[PATH] " << path << endl;

	// OPTIONAL: Print progress
	int fileSum = 0, fileCount = 0;
	for(boost::filesystem::recursive_directory_iterator end, dir(path); dir != end; ++dir) {
		if(boost::filesystem::is_regular(dir->status())) {
			fileSum++;
		}
	}

	for(boost::filesystem::recursive_directory_iterator end, dir(path); dir != end; ++dir) {
//		string fileName = boost::filesystem::canonical(dir->path()).string();
		string fileName = dir->path().string();
		if(boost::filesystem::is_regular(dir->status())) {
//			cout << "[FILE] " << fileName << endl;
			fileCount++;
			docid_t hash = docNameHash(fileName);
			hash &= 0x7FFFFFFFFFFFFFFFL;
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


	// TODO: call BStore with the map
	store = new BStore(map);

	// TODO: copy all documents to DocumentStore, rename each file to its docId

}

void SSE::remove(string document){
	
	// TODO: delete docNameHash(document) from DocumentStore
}

void SSE::add(string path){
	// TODO
	
}

void SSE::search(string keyword, vector<docid_t>& docs){
	vector<unsigned char> docIDBytes;
	store->read(keyword, docIDBytes);
	for(int i = 0; i < docIDBytes.size()/8; i++){
		docs.push_back(*(docid_t*)(&docIDBytes[i*8]));
	}
	// call BStore.get(keyword)
	// convert byte array to array of uint64_t
	// TODO: check for file with filename docNameHash(document) in
}
