//
// fileID.h
// BlindStorage
//

#ifndef __BlindStorage__fileID__
#define __BlindStorage__fileID__

#include <iostream>

#include <string>
using std::string;

#include "parameters.h"
#include "Key.h"
#include "HashMAC.h"
#include "PRF.h"

class fileID {
private:
	byte ID[FILEID_SIZE];
	int higherID;

	static byte hmacKey[];
	static byte prfKey[];

	static bool keyGenerated;
	void setupKey();
	
public:
	fileID();
	fileID(string filename);
	fileID(byte ID[]);
	~fileID();
	
	void get(byte ID[]);
};

#endif /* define(__BlindStorage__fileID__) */
