#ifndef SSE_H_
#define SSE_H_

#include <vector>
#include <iostream>
//#include<ext/hash_map>
//#include<ext/hash_set>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>

#include "AES.h"
#include "BStore.h"

using std::string;
using std::pair;
using std::istream;
using std::vector;

using std::tr1::unordered_map;
using std::tr1::unordered_set;

typedef uint64_t docid_t;

class SSE {

public:
	SSE();
//	virtual ~SSE();
	void indexGen(string path);
	void remove(string document);
	void add(string path);
	void search(string keyword, vector<docid_t>& docs);

private:
	BStore* store;
	docid_t docNameHash(string & doc);
	// map from keywords to fileNames
	unordered_map<string, unordered_set<docid_t> > map;
	// TODO: have a BStore object inside
	// TODO: function to index individual files (refactor indexGen)
};

#endif /* SSE_H_ */
