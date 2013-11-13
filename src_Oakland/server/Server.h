#ifndef __BlindStorage__Server__ 
#define __BlindStorage__Server__

#include "./../parameters.h"
#include <stdint.h>
#include <iostream>
#include <stdlib.h>

#define D_BLOCK_SIZE BLOCK_SIZE
// 4 byte size, 4 byte iv
#define T_ENTRY_LENGTH T_BLOCK_SIZE
// D_BLOCK_SIZE, 1 byte pad, 1 byte fid, 4 byte version
#define D_ENTRY_LENGTH (D_BLOCK_SIZE + 6)

#define T_LENGTH MAX_T_SIZE
#define D_LENGTH TOTAL_BLOCKS

#define REQ_T_GET		(uint8_t)0
#define REQ_T_PUT		(uint8_t)1
#define REQ_D_GET		(uint8_t)2
#define REQ_D_PUT		(uint8_t)3
#define REQ_D_MULTI_GET	(uint8_t)4
#define REQ_D_MULTI_PUT	(uint8_t)5
#define REQ_T_UPLOAD	(uint8_t)6
#define REQ_D_UPLOAD	(uint8_t)7

typedef uint32_t t_index_t;
typedef uint32_t d_index_t;

#include <fstream>
using std::ifstream;

using std::cout;
using std::endl;
using std::string;

union bstore_index_t {
	t_index_t tIndex;
	d_index_t dIndex;
};

struct Request {
	uint8_t type;
	bstore_index_t index;
};

void error(const char *msg);
void printHex(char * ptr, size_t bytes);
string reqTypeToString(uint8_t);

class Server {
public:
	Server();
	virtual ~Server();
	void setPort(int port) { this->port = port; }
	void run();

	static const int defaultPort = 9999;

private:

	bool handleRequest();

	char * tTable;
	char * dTable;
	int port;
	int verbose;
	int _connfd;
	Request request;
	void loadFile(string filename, char* input, int64_t size);
	int32_t readFileSize(string filename);

};

#endif /* defined(__BlindStorage__Server__ */
