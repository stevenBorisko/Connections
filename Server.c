#include "Connections.h"

int Server_init(HostInfo* server, size_t clientCount, size_t portNumber) {
	int retval,code;

	// socket()

	retval = socket(PF_INET, SOCK_STREAM, 0);
	code = errno;
	if(retval < 0) {
		error("Connections Server_init - socket", code);
		return retval;
	}
	server->socketInfo.fileDescriptor = retval;

	server->socketInfo.address.sin_family = AF_INET;
	server->socketInfo.address.sin_addr.s_addr = INADDR_ANY;
	server->socketInfo.address.sin_port = htons(portNumber);

	// bind()

	retval = bind(
		server->socketInfo.fileDescriptor,
		(struct sockaddr*) &server->socketInfo.address,
		sizeof(server->socketInfo.address)
	);
	code = errno;
	if(retval < 0) {
		error("Connections Server_init - bind", code);
		return retval;
	}

	// listen()

	retval = listen(
		server->socketInfo.fileDescriptor,
		clientCount
	);
	code = errno;
	if(retval < 0) {
		error("Connections Server_init - listen", code);
		return retval;
	}

	return 0;
}

int Server_accept(HostInfo* server, HostInfo* client) {
	int retval, code;

	// accept()

	retval = accept(
		server->socketInfo.fileDescriptor,
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

int Server_select(
	HostInfo* server,
	HostInfo* clients,
	int* clientStatus,
	size_t clientCount,
	struct timeval* timeout
) {
	int retval, code;
	int fdsFound = 0;
	size_t i;

	// Initialize the fd_set for select()

	fd_set hosts;
	int maxfd = 0,tempfd;

	FD_ZERO(&hosts);
	if(server) {
		maxfd = server->socketInfo.fileDescriptor;
		FD_SET(maxfd, &hosts);
	}
	for(i = 0;i < clientCount;++i) {
		tempfd = clients[i].socketInfo.fileDescriptor;
		if(tempfd > maxfd) maxfd = tempfd;
		FD_SET(tempfd,&hosts);
	}

	// select()

	retval = (server || clientCount)
		? select(maxfd+1,&hosts,NULL,NULL,timeout)
		: select(maxfd+1,NULL,NULL,NULL,timeout);
	code = errno;

	// error
	if(retval < 0) {
		error("Connections Server_select - select", code);
		return -1;
	}

	// timeout
	if(!retval)
		return 0;

	// incoming call
	clientStatus[clientCount] = 
		(server && FD_ISSET(server->socketInfo.fileDescriptor, &hosts));
	if(clientStatus[clientCount])
		++fdsFound;

	// incoming data
	for(i = 0;i < clientCount;++i) {
		clientStatus[i] = FD_ISSET(
			clients[i].socketInfo.fileDescriptor,
			&hosts
		);
		if(clientStatus[i]) ++fdsFound;
	}

	return fdsFound;
}
