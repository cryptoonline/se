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
#include "helper.h"

#include <boost/functional/hash.hpp>

#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))

typedef struct{
	size_t operator() (const std::string &str) const {return boost::hash<string>()(str);}
} stringhash;

class SSE{

public:
	SSE();
	~SSE();
	void indexgen(string directoryPath);
	void remove(string docName);
	void add(string docName);
	bool search(string keyword, vector<docid_t>& docIDs);

private:
	unordered_map<string, unordered_set<docid_t>, stringhash> map;

	BStore store;
	FileStore fstore;
	
	static bool hashKeyGenerated;
	static byte docHashKey[SSE_DIGEST_SIZE];
	
	void genPlainIndex(string directoryPath);
	
	docid_t getDocNameHash(string& docname);
	void getKeywords(byte docBytes[], size_t size, unordered_set<string, stringhash>& keywords);
	uint32_t findDocID(byte* docIDs, size_t size, docid_t docID);
	void addDocID(byte*& docIDs, size_t size, docid_t docID);
	void deleteDocID(byte* docIDs, size_t size, uint32_t docIDIndex);

	void storefile(string filename, docid_t docID);
	
	void setupKey();
//	map from keywords to fileNames
	
	// TODO: have a BStore object inside
	// TODO: function to index individual files (refactor indexGen)
};

#endif /* defined(__SearchableEncryption__SSE__) */
