//
//  Communicator.h
//  BlindStorage
//

#ifndef __BlindStorage__Communicator__
#define __BlindStorage__Communicator__

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fstream>

// 4 byte size, 4 byte iv
#define T_ENTRY_LENGTH 8
// 64 byte data, 1 byte pad, 1 byte fid, 4 byte version
#define D_ENTRY_LENGTH 70
#define T_LENGTH 65536
#define D_LENGTH 65536

#define DENTRY_DATA_LENGTH 64

#define REQ_T_GET		(uint8_t)0
#define REQ_T_PUT		(uint8_t)1
#define REQ_D_GET		(uint8_t)2
#define REQ_D_PUT		(uint8_t)3
#define REQ_D_MULTI_GET	(uint8_t)4
#define REQ_D_MULTI_PUT	(uint8_t)5

typedef uint16_t d_index_t;

using std::cout;
using std::endl;
using std::string;
using std::cerr;

struct Request {
	uint8_t type;
	uint16_t index;
};

class Communicator
{
public:
	Communicator();
	virtual ~Communicator();

	void tGet(uint16_t index, char * tEntry);
	void tPut(uint16_t index, const char * dEntry);
	void dGet(uint16_t index, char * tEntry);
	void dPut(uint16_t index, const char * dEntry);

	void setServer(string& hostname) { this->hostname = hostname; }
	void setPort(int port) { this->port = port; }
	void connect();

private:
	void writeReq(Request& req);

	int sockfd;
	string hostname;
	int port;
	int verbose;
};

#endif /* defined(__BlindStorage__Communicator__) */
