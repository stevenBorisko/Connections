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

int Server_select(
	HostInfo* host,
	HostInfo* clients,
	size_t clientCount,
	struct timeval* timeout
) {
	int retval, code;
	size_t i;

	// Initialize the fd_set for select()

	fd_set hosts;
	int maxfd = 0,tempfd;

	FD_ZERO(&hosts);
	if(host) {
		maxfd = host->socketInfo.fileDescriptor;
		FD_SET(maxfd, &hosts);
	}
	for(i = 0;i < clientCount;++i) {
		tempfd = clients[i].socketInfo.fileDescriptor;
		if(tempfd > maxfd) maxfd = tempfd;
		FD_SET(tempfd,&hosts);
	}

	// select()

	retval = (host || clientCount)
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
		return clientCount + 1;

	// incoming call
	if(FD_ISSET(host->socketInfo.fileDescriptor, &hosts))
		return clientCount;

	// incoming data
	for(i = 0;i < clientCount;++i)
		if(FD_ISSET(clients[i].socketInfo.fileDescriptor,&hosts))
			return (int)i;

	error("Connections Server_select - could not find fd",0);
	return -1;
}
