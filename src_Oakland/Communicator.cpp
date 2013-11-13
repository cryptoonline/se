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

#include "Communicator.h"

using std::cout;
using std::endl;

Communicator::Communicator()
{
	hostname = "localhost";//"seclab-central.cs.illinois.edu";
	sockfd = 0;
	port = 9999;
	verbose = 0;
}

Communicator::~Communicator()
{
	close(sockfd);
}

void Communicator::writeReq(Request& req)
{
	int n = write(sockfd, &req, sizeof(Request));
	sktError(n, sizeof(Request));
	if(verbose)
		cout << "[REQ] wrote request to socket" << endl;
}

void Communicator::tGet(t_index_t index, char * tEntry)
{
	int n;
	Request req;
	req.type = REQ_T_GET;
	req.index.tIndex = index;

	writeReq(req);

	n = read(sockfd, tEntry, T_ENTRY_LENGTH);
	cout << "TEntry in tGet " << endl;
	for(int i = 0; i < 12; i++)
		printf("%02X ", (unsigned char)tEntry[i]);
	cout << endl;

	cout << "Index in tGet is " << index << endl;

	sktError(n, T_ENTRY_LENGTH);
	if(verbose)
		cout << "[T_GET] received TEntry" << endl;
}

void Communicator::tPut(t_index_t index, const char * tEntry)
{
	int n;
	Request req;
	req.type = REQ_T_PUT;
	req.index.tIndex = index;

	writeReq(req);

	cout << "TEntry in tPut " << endl;
	for(int i = 0; i < 12; i++)
		printf("%02X ", tEntry[i]);
	cout << endl;
	cout << "Index in tPut is " << index << endl;

	n = write(sockfd, tEntry, T_ENTRY_LENGTH);
	sktError(n, T_ENTRY_LENGTH);
	if(verbose)
		cout << "[T_PUT] wrote tEntry" << endl;
}

void Communicator::dGet(d_index_t index, char * dEntry)
{
	int n;
	Request req;
	req.type = REQ_D_GET;
	req.index.dIndex = index;

	writeReq(req);

	n = read(sockfd, dEntry, D_ENTRY_LENGTH);
	sktError(n, D_ENTRY_LENGTH);
	if(verbose)
		cout << "[D_GET] received DEntry" << endl;
}

void Communicator::dPut(d_index_t index, const char * dEntry)
{
	int n;
	Request req;
	req.type = REQ_D_PUT;
	req.index.dIndex = index;

	writeReq(req);
	n = write(sockfd, dEntry, D_ENTRY_LENGTH);
	sktError(n, D_ENTRY_LENGTH);
	if(verbose)
		cout << "[D_PUT] wrote DEntry" << endl;
}

void Communicator::dPut(d_index_t * indices, const char ** dEntries, d_index_t num)
{
	int n;
	Request req;
	req.type = REQ_D_MULTI_PUT;
	req.index.dIndex = num;
	writeReq(req);
	// write (index, dEntry pairs)
	for(d_index_t i = 0; i < num; i++) {
		n = write(sockfd, &indices[i], sizeof(d_index_t));
		sktError(n, sizeof(d_index_t));
		n = write(sockfd, dEntries[i], D_ENTRY_LENGTH);
		sktError(n, D_ENTRY_LENGTH);
	}

}

void Communicator::dGet(d_index_t * indices, char ** dEntries, d_index_t num)
{
	int n;
	Request req;
	req.type = REQ_D_MULTI_GET;
	req.index.dIndex = num;
	writeReq(req);
	// write entire array of indices
	n = write(sockfd, indices, num * sizeof(d_index_t));
	sktError(n, num * sizeof(d_index_t));
	// read into dEntries
	for(d_index_t i = 0; i < num; i++) {
		n = read(sockfd, dEntries[i], D_ENTRY_LENGTH);
		sktError(n, D_ENTRY_LENGTH);
	}

}

void Communicator::tUpload(const char ** tEntries)
{
	int n;
	Request req;
	req.type = REQ_T_UPLOAD;
	// do something with req.index? (maybe validate T/D length)
	writeReq(req);
	for(int i = 0; i < T_LENGTH; i++) {
		n = write(sockfd, tEntries[i], T_ENTRY_LENGTH);
		sktError(n, T_ENTRY_LENGTH);
	}
}

void Communicator::dUpload(const char ** dEntries)
{
	int n;
	Request req;
	req.type = REQ_D_UPLOAD;
	// req.index...
	writeReq(req);
	for(int i = 0; i < D_LENGTH; i++) {
		n = write(sockfd, dEntries[i], D_ENTRY_LENGTH);
		sktError(n, D_ENTRY_LENGTH);
	}
}

void Communicator::connect()
{
	struct sockaddr_in serveraddr;
	struct hostent *server;

	/* socket: create the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		cerr << "ERROR opening socket" << endl;

	/* gethostbyname: get the server's DNS entry */
	server = gethostbyname(hostname.c_str());
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host as %s\n", hostname.c_str());
		exit(0);
	}

	/* build the server's Internet address */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
			(char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(port);

	/* connect: create a connection with the server */
	if (::connect(sockfd, (const sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		cerr << "ERROR connecting" << endl;
	if(verbose) {
		cout << "connected to " + hostname << ":" << port << endl;
	}
}


