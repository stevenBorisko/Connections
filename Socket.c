#include "Connections.h"

int Write(HostInfo* host, void* data, size_t bytes) {
	int retval = write(host->socketInfo.fileDescriptor, data, bytes);
	int code = errno;

	if(retval < 0)
		error("Connections Write - write",code);

	return retval;
}

int Read(HostInfo* host, void* data, size_t bytes) {
	int retval = read(host->socketInfo.fileDescriptor, data, bytes);
	int code = errno;

	if(retval < 0)
		error("Connections Read - read",code);

	return retval;
}

int Close(HostInfo* host) {
	int retval = close(host->socketInfo.fileDescriptor);
	int code = errno;

	if(retval < 0)
		error("Connections Close - close",code);

	return retval;
}
