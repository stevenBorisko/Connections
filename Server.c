#include "Connections.h"

int Server_init(HostInfo* host, size_t clientCount, size_t portNumber) {
	int retval,code;

	// socket()

	retval = socket(PF_INET, SOCK_STREAM, 0);
	code = errno;
	if(retval < 0) {
		error("Connections Server_init - socket", code);
		return retval;
	}
	host->socketInfo.fileDescriptor = retval;

	host->socketInfo.address.sin_family = AF_INET;
	host->socketInfo.address.sin_addr.s_addr = INADDR_ANY;
	host->socketInfo.address.sin_port = htons(portNumber);

	// bind()

	retval = bind(
		host->socketInfo.fileDescriptor,
		(struct sockaddr*) &host->socketInfo.address,
		sizeof(host->socketInfo.address)
	);
	code = errno;
	if(retval < 0) {
		error("Connections Server_init - bind", code);
		return retval;
	}

	// listen()

	retval = listen(
		host->socketInfo.fileDescriptor,
		clientCount
	);
	code = errno;
	if(retval < 0) {
		error("Connections Server_init - listen", code);
		return retval;
	}

	return 0;
}

int Server_accept(HostInfo* host, HostInfo* client) {
	int retval, code;

	// accept()

	retval = accept(
		host->socketInfo.fileDescriptor,
		(struct sockaddr*) &client->socketInfo.address,
		&client->socketInfo.length
	);
	code = errno;
	if(retval < 0) {
		error("Connections Server_accept - accept", code);
		return retval;
	}
	client->socketInfo.fileDescriptor = retval;

	return 0;
}
