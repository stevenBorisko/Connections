# make <Connections path>/Connections.o a prerequisite for your executable

CONNECTIONS_PATH := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

_CONNECTIONS_OBJS := Client.o Server.o Socket.o
CONNECTIONS_OBJS := $(foreach OBJ,$(_CONNECTIONS_OBJS),$(CONNECTIONS_PATH)$(OBJ))

$(CONNECTIONS_PATH)Connections.o: $(CONNECTIONS_OBJS)
	$(LD) -r $(CONNECTIONS_OBJS) -o $@

$(CONNECTIONS_PATH)%.o: $(CONNECTIONS_PATH)%.c $(CONNECTIONS_PATH)Connections.h
	$(CC) -c $< -o $@ $(CFLAGS)

ConnectionsClean:
	rm -f $(CONNECTIONS_PATH)*.o
