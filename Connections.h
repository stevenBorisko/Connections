#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../Degub/Degub.h"

//----------------------------------------------------------------------------//
// --- Macro Definitions ---
//----------------------------------------------------------------------------//

#define TESP_PORT_NO 10101

//----------------------------------------------------------------------------//
// --- Structure Declarations ---
//----------------------------------------------------------------------------//

/*
--- struct SocketInfo ---

fileDescriptor (int)
	- The file descriptor pertaining to the socket.
	- If the socket is a different computer, this file descriptor may be
	written to read from.
address (struct sockaddr_in)
	- Network information about the host pertaining to the socket.
length (socklen_t)
	- Length of `address`.
	- May be changed in some socket API calls depending on the host

Socket information about a host.
	- This code base was written so whomever is using it does not have to
	worry about the lower level socket API calls, so don't worry too much
	about this structure. I don't know how most of it works either...
*/
typedef struct {
	int fileDescriptor;
	struct sockaddr_in address;
	socklen_t length;
} SocketInfo;

/*
--- struct HostInfo ---

socketInfo (SocketInfo)
	- Socket information about the host for communication with the host.
data (void*)
	- User data pertaining to the host.
	- Memory allocation and deallocation is on you.

All information about a host.
	- May be a server or client.
	- Includes only the basics to support reading and writing to another
	host.
*/
typedef struct {
	SocketInfo socketInfo;
	void* data;
} HostInfo;

//----------------------------------------------------------------------------//
// --- Socket Function Declarations ---
//----------------------------------------------------------------------------//

/*
--- Write ---

Write data to a host.
*/
int Write(HostInfo* host, void* data, size_t bytes);

/*
--- Read ---

Read data to a host.
*/
int Read(HostInfo* host, void* data, size_t bytes);

/*
--- Close ---

Close the connection to a hast.
*/
int Close(HostInfo* host);

//----------------------------------------------------------------------------//
// --- Server Function Declarations ---
//----------------------------------------------------------------------------//

/*
--- Server_init ---

Initialize your machine as a server to allow incoming calls from other machines.
*/
int Server_init(HostInfo* host, size_t clientCount, size_t portNumber);

/*
--- Server_accept ---

Accept an incoming call from a client on the open socket specified in `host`.
*/
int Server_accept(HostInfo* host, HostInfo* client);

//----------------------------------------------------------------------------//
// --- Client Function Declarations ---
//----------------------------------------------------------------------------//

/*
--- Client_connect ---

Open a connection with a server.
*/
int Client_connect(HostInfo* host, char* hostname, size_t portNumber);

#endif
