#include "Connections.h"

int Client_connect(HostInfo* host, char* hostname, size_t portNumber) {
	int retval,code;

	// socket()

	retval = socket(PF_INET, SOCK_STREAM, 0);
	code = errno;
	if(retval < 0) {
		error("Connections Client_connect - socket", code);
		return retval;
	}
	host->socketInfo.fileDescriptor = retval;

	// gethostbyname()

	struct hostent* server_hostent;
	server_hostent = gethostbyname(hostname);
	code = errno;
	if(!server_hostent) {
		error("Connections Client_connect - gethostbyname", code);
		return retval;
	}

	memset(&host->socketInfo.address, 0, sizeof(host->socketInfo.address));
	host->socketInfo.address.sin_family = AF_INET;
	memmove(
		&host->socketInfo.address.sin_addr.s_addr,
		server_hostent->h_addr,
		server_hostent->h_length
	);
	host->socketInfo.address.sin_port = htons(portNumber);

	// connect()

	retval = socket(PF_INET, SOCK_STREAM, 0);
	retval = connect(
		host->socketInfo.fileDescriptor,
		(struct sockaddr*) &host->socketInfo.address,
		sizeof(host->socketInfo.address)
	);
	code = errno;
	if(retval < 0) {
		error("Connections Client_connect - connect", code);
		return retval;
	}

	return 0;
}
