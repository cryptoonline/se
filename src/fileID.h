//
// fileID.h
// BlindStorage
//

#ifndef __BlindStorage__fileID__
#define __BlindStorage__fileID__

#include "parameters.h"

class fileID {
private:
	byte higherID[HIGHERFID_SIZE];
	byte lowerID[LOWERFID_SIZE];
	byte ID[FILEID_SIZE];

	static byte hashKey[];
	static byte prpKey[];



public:
	fileID();
	~fileID();
};

#endif /* define(__BlindStorage__fileID__) */
