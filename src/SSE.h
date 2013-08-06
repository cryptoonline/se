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
#include <boost/cstdint.hpp>

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

#define SET_BIT(var, pos) ( (var) |= 1ULL << (pos) )
#define CLEAR_BIT(var, pos) ( (var) &= ~(1ULL << (pos)) )

typedef struct{
	size_t operator() (const std::string &str) const {return boost::hash<string>()(str);}
} stringhash;

class SSE{

public:
	SSE();
	~SSE();
	void indexgen(string directoryPath, double& execTime);
	void remove(docid_t docName, double& execTime);
	void add(docid_t docName, string docFileName, double& execTime);
	bool search(string keyword, vector<docid_t>& docIDs, double& execTime);

private:
	unordered_map<string, unordered_set<docid_t>, stringhash> map;

	BStore store;
	FileStore fstore;
	
	bool hashKeyGenerated;
	byte docHashKey[HMAC_KEY_SIZE];

	HashMAC hashMac;

	void genPlainIndex(string directoryPath);
	
	uint64_t getDocNameHash(string docname);

	bool retrieveIndex0(string keyword, vector<docid_t>& docIDs);
	bool retrieveIndex1(string keywords, vector<docid_t>& docIDs);

	void getKeywords(byte docBytes[], size_t size, unordered_set<string, stringhash>& keywords);
	int32_t findDocID(byte* docIDs, size_t size, docid_t docID);
	void addDocID(byte*& docIDs, size_t size, docid_t docID);
	void deleteDocID(byte* docIDs, size_t size, uint32_t docIDIndex);

	void storefile(string filename, uint64_t docID, double& cryptoduration);
	
	void setupKey();
//	map from keywords to fileNames
	
	// TODO: have a BStore object inside
	// TODO: function to index individual files (refactor indexGen)
};

#endif /* defined(__SearchableEncryption__SSE__) */
