//
// optBStore.h
// BlindStorage
//

#ifndef __BlindStorage__optBStore__
#define __BlindStorage__optBStore__

#include "PRSubset.h"
#include "optT.h"
#include "Ddisk.h"
#include "parameters.h"

class optBStore{
private:
	Ddisk D;
	optT T;
	
public:
	optBStore();
	~optBStore();
	void add(string filename, byte fileBytes[], size_t size);
	void finalize();
};

#endif /* defined(__BlindStorage__optBStore__) */
