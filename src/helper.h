//
// helper.h
// BlindStorage
//

#ifndef __BlindStorage__helper__
#define __BlindStorage__helper__

#include <iostream>
#include <iomanip>
#include <stdlib.h>

#include <sstream>
using std::cout;
using std::endl;
using std::cerr;
using std::hex;

using std::stringstream;
using std::istringstream;
using std:: string;

#include <fstream>
using std::ifstream;

#include <sys/stat.h>

#include "parameters.h"

void hexStringToByteArray(string hexstring, unsigned char bytes[]);
void gen_random(byte* s, const int len);
size_t readFileSize(string path);
void readFile(string path, byte fileBytes[], size_t size);

#endif /* defined(__BlindStorage__helper__) */
