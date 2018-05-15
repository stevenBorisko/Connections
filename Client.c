#include "Connections.h"

int Client_connect(HostInfo* server, char* hostname, size_t portNumber) {
	int retval,code;

	// socket()

	retval = socket(PF_INET, SOCK_STREAM, 0);
	code = errno;
	if(retval < 0) {
		error("Connections Client_connect - socket", code);
		return retval;
	}
	server->socketInfo.fileDescriptor = retval;

	// gethostbyname()

	struct hostent* server_hostent;
	server_hostent = gethostbyname(hostname);
	code = errno;
	if(!server_hostent) {
		error("Connections Client_connect - gethostbyname", code);
		return retval;
	}

	memset(&server->socketInfo.address, 0, sizeof(server->socketInfo.address));
	server->socketInfo.address.sin_family = AF_INET;
	memmove(
		&server->socketInfo.address.sin_addr.s_addr,
		server_hostent->h_addr,
		server_hostent->h_length
	);
	server->socketInfo.address.sin_port = htons(portNumber);

	// connect()

	retval = socket(PF_INET, SOCK_STREAM, 0);
	retval = connect(
		server->socketInfo.fileDescriptor,
		(struct sockaddr*) &server->socketInfo.address,
		sizeof(server->socketInfo.address)
	);
	code = errno;
	if(retval < 0) {
		error("Connections Client_connect - connect", code);
		return retval;
	}

	return 0;
}
