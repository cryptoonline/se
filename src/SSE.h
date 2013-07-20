//
// Searchable Encryption
// SSE.h
// 

#ifndef __SearchableEncryption__SSE__
#define __SearchableEncryption__SSE__

#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::istream;
using std::vector;

#include <cstdlib>
//#include<ext/hash_map>
//#include<ext/hash_set>
//#include <cryptopp/hex.h>
//#include <cryptopp/sha.h>
//#include <cryptopp/base64.h>

#include <fstream>
using std::ifstream;

#include <algorithm>

#include <string>

#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <tr1/unordered_map>
using std::tr1::unordered_map;

#include <tr1/unordered_set>
using std::tr1::unordered_set;

#include "sse_parameters.h"

#include "SSE.h"

#include "OnlineSession.h"
#include "BStore.h"
#include "HashMAC.h" 
#include "Key.h"
#include "FileStore.h"

#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))

class SSE{

public:
	SSE();
	~SSE();
	void indexgen(string directoryPath);
	void remove(string docName);
	void add(string docName);
	bool search(string keyword, vector<docid_t>& docIDs);

private:
	static byte docHashKey[SSE_DIGEST_SIZE];
	static bool hashKeyGenerated;
	void genPlainIndex(string directoryPath);
	
	BStore store;
	FileStore fstore;
	docid_t getDocNameHash(string& docname);
	void setupKey();

	void getKeywords(byte docBytes[], size_t size, vector<string>& keywords);
	
//	map from keywords to fileNames
	unordered_map<string, unordered_set<docid_t> > map;
	
	// TODO: have a BStore object inside
	// TODO: function to index individual files (refactor indexGen)
};

#endif /* defined(__SearchableEncryption__SSE__) */
