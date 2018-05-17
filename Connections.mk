# make <Connections path>/Connections.o a prerequisite for your executable

# --- Definitions ---

CONNECTIONS_PATH := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

_CONNECTIONS_DEPS := Connections.h ../Degub.h
CONNECTIONS_DEPS := $(foreach OBJ,$(_CONNECTIONS_DEPS),$(CONNECTIONS_PATH)$(OBJ))

_CONNECTIONS_OBJS := Client.o Server.o Socket.o
CONNECTIONS_OBJS := $(foreach OBJ,$(_CONNECTIONS_OBJS),$(CONNECTIONS_PATH)$(OBJ))

# --- Rules ---

# Main object
$(CONNECTIONS_PATH)Connections.o: $(CONNECTIONS_OBJS)
	$(LD) -r $(CONNECTIONS_OBJS) -o $@

# Sub objects
$(CONNECTIONS_PATH)%.o: $(CONNECTIONS_PATH)%.c $(CONNECTIONS_DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean
ConnectionsClean:
	rm -f $(CONNECTIONS_PATH)*.o
