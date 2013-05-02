#ifndef BCLIENT_H_
#define BCLIENT_H_

#include <cstdlib>

using std::string;

#define D_BLOCK_SIZE 1024
// 4 byte size, 4 byte iv
#define T_ENTRY_LENGTH 8
// // D_BLOCK_SIZE, 1 byte pad, 1 byte fid, 4 byte version
#define D_ENTRY_LENGTH (D_BLOCK_SIZE + 6)

#define T_LENGTH 65536
#define D_LENGTH 65536

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
void printHex(uint8_t * ptr, size_t bytes);
string reqTypeToString(uint8_t);

using namespace std;

class Communicator
{
public:
	Communicator();
	virtual ~Communicator();

	void tGet(t_index_t index, char * tEntry);
	void tPut(t_index_t index, const char * tEntry);
	void dGet(d_index_t index, char * dEntry);
	void dPut(d_index_t index, const char * dEntry);

	void dPut(d_index_t * indices, const char ** dEntries, d_index_t num);
	void dGet(d_index_t * indices, char ** dEntries, d_index_t num);

	void tUpload(const char ** tEntries);
	void dUpload(const char ** dEntries);

	void setServer(string& hostname) { this->hostname = hostname; }
	void setPort(int port) { this->port = port; }
	void connect();

private:
	void writeReq(Request& req);

	inline void sktError(int transferred, int expected) {
		if(transferred < expected) {
			std::cerr << "ERROR Socket I/O" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	int sockfd;
	string hostname;
	int port;
	int verbose;
};

#endif /* BCLIENT_H_ */
