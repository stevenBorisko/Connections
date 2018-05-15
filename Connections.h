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

#define TEST_PORT_NO 10101
#define TEST_BUFF_SIZE 128

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

host (HostInfo*)
	- Host to which data should be written
data (void*)
	- Buffer from which data should be read
bytes (size_t)
	- Number of bytes to be sent starting at `data` to `host`

return (int)
	-1: error
	else: Number of bytes written

Write data to a host.
*/
int Host_write(HostInfo* host, void* data, size_t bytes);

/*
--- Read ---

host (HostInfo*)
	- Host from which data should be read
data (void*)
	- Buffer to which data should be written
bytes (size_t)
	- Number of bytes to be read into `data` from `host`

return (int)
	-1: Error
	else: Number of bytes read

Read data to a host.
*/
int Host_read(HostInfo* host, void* data, size_t bytes);

/*
--- Close ---

host (HostInfo*)
	- Connection to close

return (int)
	-1: Error
	0: Success

Close the connection to a hast.
*/
int Host_close(HostInfo* host);

//----------------------------------------------------------------------------//
// --- Server Function Declarations ---
//----------------------------------------------------------------------------//

/*
--- Server_init ---

server (HostInfo*)
	- HostInfo struct allocated before function call.
	- This memory will be filled upon success with information about
	connections on your machine.
clientCount (size_t)
	- Number of clients to allow connection.
	- This gets passed as a parameter to listen().
portNumber (size_t)
	- Port number on which connections will be accepted.

return (int)
	0: Success
	else: Error

Initialize your machine as a server to allow incoming calls from other machines.
*/
int Server_init(HostInfo* server, size_t clientCount, size_t portNumber);

/*
--- Server_accept ---

server (HostInfo*)
	- HostInfo struct allocated and initialized in `Server_init`.
client (HostInfo*)
	- HostInfo struct allocated before function call.
	- This memory will be filled upon success with information about
	an accepted connection with a client.

return (int)
	0: Success
	else: Error

Accept an incoming call from a client on the open socket specified in `server`.
*/
int Server_accept(HostInfo* server, HostInfo* client);

/*
--- Server_select ---

server (HostInfo*)
	- HostInfo struct allocated and initialized in `Server_init`.
	- This function will wait on incoming calls from this listening socket.
clients (HostInfo*)
	- List of clients on which this function will wait for incoming data.
clientCount (size_t)
	- Number of elements int the list `clients`.
timeout (struct timeval)
	- Amount of time this function will block with no actions before
	returning

return (int)
	-1: Error
	0..<`clientCount`: Newest action was incoming data from a client in
		`clients`. This value is the index of that client in the list.
	`clientCount`: Newest action was an incoming call on `server`.
	`clientCount` + 1: Function timed out before any action occurred.

Select incoming data or connections from a set of hosts
	- Outcomes include:
		* Incoming data from one of the clients in `clients`
		* Incoming connection on `server`
		* Timeout
	- In the case of an incoming call, `Server_accept` is not called.
	That must be done manually after the function's return.
*/
int Server_select(
	HostInfo* server,
	HostInfo* clients,
	size_t clientCount,
	struct timeval* timeout
);

//----------------------------------------------------------------------------//
// --- Client Function Declarations ---
//----------------------------------------------------------------------------//

/*
--- Client_connect ---

server (HostInfo*)
	- HostInfo struct allocated before function call
	- This memory will be filled upon success with information about
	an accepted connection with a server
hostname (char*)
	- Hostname of the server with which a connection will be made
portNumber (size_t)
	- Port number on which a connection will be made

return (int)
	0: Success
	else: Error

Open a connection with a server.
*/
int Client_connect(HostInfo* server, char* hostname, size_t portNumber);

#endif
