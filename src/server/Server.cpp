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

#include "Server.h"

using std::cout;
using std::endl;
using std::string;

// initialize T and D tables in memory, possibly read them from disk
Server::Server()
{
	// allocate T and D contiguously
	/*
	tTable = new char * [T_LENGTH];
	tTable[0] = new char[T_LENGTH * T_ENTRY_LENGTH];
	for(int i = 1; i < T_LENGTH; i++) {
		tTable[i] = tTable[0] + i * T_ENTRY_LENGTH;
	}

	dTable = new char * [D_LENGTH];
	dTable[0] = new char[D_LENGTH * D_ENTRY_LENGTH];
	for(int i = 1; i < D_LENGTH; i++) {
		dTable[i] = dTable[0] + i * D_ENTRY_LENGTH;
	}
	*/

	int64_t TSize = readFileSize("/home/naveed2/BStore/data/T");
	int64_t DSize = readFileSize("/home/naveed2/BStore/data/D");
	tTable = new char[TSize];
	dTable = new char[DSize];

	cout << "Reading T: " << endl;
	loadFile("/home/naveed2/BStore/data/T", tTable, TSize);
	cout << "Reading T completed." << endl;

	cout << "Reading D: " << endl;
	loadFile("/home/naveed2/BStore/data/D", dTable, DSize);
	cout << "Reading D completed." << endl;

	cout << "Server T Record " << endl;
	printHex(&tTable[3287*12], 12);
	cout << endl;
	port = defaultPort;
	verbose = 1;
	_connfd = 0;
}

Server::~Server()
{
	delete [] tTable;
	delete [] dTable;
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void printHex(char * ptr, size_t bytes)
{
	for(unsigned int i = 0; i < bytes; i++) {
		printf("%02X ", (unsigned char)*(ptr + i));
	}
	cout << endl;
}

string reqTypeToString(uint8_t reqType)
{
	switch(reqType) {
		case REQ_T_GET:
			return string("REQ_T_GET");
			break;
		case REQ_D_GET:
			return string("REQ_D_GET");
			break;
		case REQ_T_PUT:
			return string("REQ_T_PUT");
			break;
		case REQ_D_PUT:
			return string("REQ_D_PUT");
			break;
		case REQ_D_MULTI_GET:
			return string("REQ_D_MULTI_GET");
			break;
		case REQ_D_MULTI_PUT:
			return string("REQ_D_MULTI_PUT");
			break;
		case REQ_T_UPLOAD:
			return string("REQ_T_UPLOAD");
			break;
		case REQ_D_UPLOAD:
			return string("REQ_D_UPLOAD");
			break;
		default:
			return string("ERROR");
	}
}

/*
 * Handles individual Request from a client.
 * Returns false if there are errors and this connection needs to be terminated.
 */
bool Server::handleRequest()
{
	int n, num;
	bstore_index_t * indices;
	// we shouldn't need to zero buffer
	n = read(_connfd, &request, sizeof(request));
	if (n < 0) {
		perror("ERROR reading from socket");
		return false;
	}

	if(verbose) {
		cout << "received " << n << " bytes, requestType: " << reqTypeToString(request.type) << ", index: " << request.index.dIndex << endl;
	}

	switch(request.type) {
		case REQ_T_GET:
//			cout << "[REQ_T_GET] idx:" << request.index << endl;
			n = write(_connfd, &tTable[request.index.tIndex*T_BLOCK_SIZE], T_ENTRY_LENGTH);
			cout << "write() done, n:" << n << endl;
			if(n < 0) {
				perror("ERROR writing to socket");
				return false;
			}
			break;

		case REQ_D_GET:
			n = write(_connfd, &dTable[request.index.dIndex*BLOCK_SIZE], D_ENTRY_LENGTH);
			if(n < 0) {
				perror("ERROR writing to socket");
				return false;
			}
			break;

		case REQ_T_PUT:
//			cout << "[REQ_T_PUT] idx:" << request.index << ", cur_sz:" << tTable[request.index].size << ", cur_iv:" << tTable[request.index].iv << endl;
			n = read(_connfd, &tTable[request.index.tIndex*T_BLOCK_SIZE], T_ENTRY_LENGTH);
			if (n < T_ENTRY_LENGTH) {
				error("ERROR reading from socket");
				return false;
			}
//			cout << "read() done, n: " << n << ", size:" << tTable[request.index].size << ", iv:" << tTable[request.index].iv << endl;
			break;

		case REQ_D_PUT:
			n = read(_connfd, &dTable[request.index.dIndex*BLOCK_SIZE], D_ENTRY_LENGTH);
			if (n < D_ENTRY_LENGTH) {
				error("ERROR reading from socket");
				return false;
			}
			break;

		case REQ_D_MULTI_GET:
			num = request.index.dIndex;

			// read indices
			indices = new bstore_index_t [num];
			n = read(_connfd, indices, num * sizeof(d_index_t));
			if (n < num * (int)sizeof(d_index_t)) {
				perror("ERROR reading from socket");
				return false;
			}

			// for each index, write back entry
			for(int i = 0; i < num; i++) {
				n = write(_connfd, &dTable[indices[i].dIndex*BLOCK_SIZE], D_ENTRY_LENGTH);
				if(n < 0) {
					perror("ERROR reading from socket");
					return false;
				}
			}
			delete [] indices;
			break;

		case REQ_D_MULTI_PUT:
			num = request.index.dIndex;
			// read pairs: (index, DEntry)
			for(int i = 0; i < num; i++) {
				// read index
				d_index_t idx;
				n = read(_connfd, &idx, sizeof(d_index_t));
				if (n < (int)sizeof(d_index_t)) {
					perror("ERROR reading from socket");
					return false;
				}
				cout << "[MULTI_PUT] read idx = " << idx << endl;
				// read DEntry
				n = read(_connfd, &dTable[idx*BLOCK_SIZE], D_ENTRY_LENGTH);
				if(n < D_ENTRY_LENGTH) {
					perror("ERROR reading from socket");
					return false;
				}
				cout << "[MULTI PUT] read DEntry, bytes = " << n << endl;
			}
			break;

		case REQ_T_UPLOAD:
//			n = read(_connfd, tTable[0], T_LENGTH * T_ENTRY_LENGTH);
//			if(n < T_LENGTH * T_ENTRY_LENGTH) {
//				perror("ERROR reading from socket");
//				return false;
//			}
//			cout << "[T UPLOAD] bytes = " << n << endl;
			for(int i = 0; i < T_LENGTH; i++) {
				n = read(_connfd, &tTable[i*T_BLOCK_SIZE], T_ENTRY_LENGTH);
				if(n < T_ENTRY_LENGTH) {
					perror("ERROR reading from socket");
					return false;
				}
				cout << "[T UPLOAD] i = " << i << ", bytes = " << n << endl;
			}
			break;

		case REQ_D_UPLOAD:
//			n = read(_connfd, dTable[0], D_LENGTH * D_ENTRY_LENGTH);
//			if(n < D_LENGTH * D_ENTRY_LENGTH) {
//				perror("ERROR reading from socket");
//				return false;
//			}
//			cout << "[D UPLOAD] bytes = " << n << endl;
			for(int i = 0; i < D_LENGTH; i++) {
				n = read(_connfd, &dTable[i*BLOCK_SIZE], D_ENTRY_LENGTH);
				if(n < D_ENTRY_LENGTH) {
					perror("ERROR reading from socket");
					return false;
				}
				cout << "[D UPLOAD] i = " << i << ", bytes = " << n << endl;
			}
			break;

		default:
			perror("ERROR invalid request type");
			return false;
	}
	return true;
}

void Server::loadFile(string filename, char* input, int64_t size){
	ifstream file(filename.c_str(), std::ios_base::binary | std::ios_base::in);
	file.seekg(0, std::ios::beg);
	file.read(input, size);
	file.close();
}

int32_t Server::readFileSize(string filename){
	ifstream file(filename.c_str(), std::ios_base::binary | std::ios_base::in | std::ios_base::ate);
	int32_t size = file.tellg();
	file.close();
	return size;
}

void Server::run()
{
	int listenfd; /* listening socket */
//	int connfd; /* connection socket */
	socklen_t clientlen; /* byte size of client's address */
	struct sockaddr_in serveraddr; /* server's addr */
	struct sockaddr_in clientaddr; /* client addr */
	struct hostent *hostp; /* client host info */
	char *hostaddrp; /* dotted decimal host addr string */
	int optval; /* flag value for setsockopt */

	/* socket: create a socket */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
		error("ERROR opening socket");
	if(verbose) {
		cout << "socket() complete" << endl;
	}

	/* setsockopt: Handy debugging trick that lets
	 * us rerun the server immediately after we kill it;
	 * otherwise we have to wait about 20 secs.
	 * Eliminates "ERROR on binding: Address already in use" error.
	 */
	optval = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
			(const void *)&optval , sizeof(int));

	/* build the server's internet address */
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET; /* we are using the Internet */
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); /* accept reqs to any IP addr */
	serveraddr.sin_port = htons((unsigned short)port); /* port to listen on */

	/* bind: associate the listening socket with a port */
	if (bind(listenfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
		error("ERROR on binding");
	if(verbose) {
		cout << "bind() complete, port number: " << port << endl;
	}

	/* listen: make it a listening socket ready to accept connection requests */
	if (listen(listenfd, 5) < 0) /* allow 5 requests to queue up */
		error("ERROR on listen");
	if(verbose) {
		cout << "listen() complete" << endl;
	}

	/* main loop: wait for a connection request, echo input line,
	     then close connection. */
	clientlen = sizeof(clientaddr);
	while(true) {
		cout << "while(true)" << endl;
		/* accept: wait for a connection request */
		_connfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clientlen);
		if (_connfd < 0)
			error("ERROR on accept");

		if(verbose) {
			/* gethostbyaddr: determine who sent the message */
			hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
					sizeof(clientaddr.sin_addr.s_addr), AF_INET);
			if (hostp == NULL)
				error("ERROR on gethostbyaddr");
			hostaddrp = inet_ntoa(clientaddr.sin_addr);
			if (hostaddrp == NULL)
				error("ERROR on inet_ntoa\n");
			printf("server established connection with %s (%s)\n",
					hostp->h_name, hostaddrp);
		}

		// need to continue handling requests on the same connection from the same client until it disconnects
		while(handleRequest());

		close(_connfd);
	}
}


/*
#ifdef BSERVER_MAIN

int main(int argc, char * argv[])
{
	Server bserver;

	if (argc >= 2) {
		int port = atoi(argv[1]);
		bserver.setPort(port);
	}

	bserver.run();
}
#endif
*/
