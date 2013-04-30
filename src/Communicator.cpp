//
//  Communicator.cpp
//  BlindStorage
//

#include "Communicator.h"

Communicator::Communicator()
{
	hostname = "seclab-central.cs.illinois.edu";
	sockfd = 0;
	port = 9999;
	verbose = 1;
}

Communicator::~Communicator()
{
	close(sockfd);
}

void Communicator::writeReq(Request& req)
{
	int n = write(sockfd, &req, sizeof(req));
	if (n < 0)
		cerr << "ERROR writing to socket";
	if(verbose)
		cout << "[REQ] wrote request to socket" << endl;
}

void Communicator::tGet(uint16_t index, char * tEntry)
{
	int n;
	Request req;
	req.type = REQ_T_GET;
	req.index = index;

	writeReq(req);

	n = read(sockfd, tEntry, T_ENTRY_LENGTH);
	if (n < 0)
		cerr << "ERROR reading from socket";
	if(verbose)
		cout << "[T_GET] received TEntry" << endl;
}

void Communicator::tPut(uint16_t index, const char * tEntry)
{
	int n;
	Request req;
	req.type = REQ_T_PUT;
	req.index = index;

	writeReq(req);
	n = write(sockfd, tEntry, T_ENTRY_LENGTH);
	if(n < 0)
		cerr << "ERROR writing to socket";
	if(verbose)
		cout << "[T_PUT] wrote tEntry" << endl;
}

void Communicator::dGet(uint16_t index, char * dEntry)
{
	int n;
	Request req;
	req.type = REQ_D_GET;
	req.index = index;

	writeReq(req);

	n = read(sockfd, dEntry, D_ENTRY_LENGTH);
	if (n < 0)
		cerr << "ERROR reading from socket";
	if(verbose)
		cout << "[D_GET] received DEntry" << endl;
}

void Communicator::dPut(uint16_t index, const char * dEntry)
{
	int n;
	Request req;
	req.type = REQ_D_PUT;
	req.index = index;

	writeReq(req);
	n = write(sockfd, dEntry, D_ENTRY_LENGTH);
	if(n < 0)
		cerr << "ERROR writing to socket";
	if(verbose)
		cout << "[D_PUT] wrote DEntry" << endl;
}

void Communicator::connect()
{
	struct sockaddr_in serveraddr;
	struct hostent *server;

	/* socket: create the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		cerr << "ERROR opening socket";

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
		cerr << "ERROR connecting";
	if(verbose) {
		cout << "connected to " + hostname << ":" << port << endl;
	}
}


