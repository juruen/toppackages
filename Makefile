SERVERSOURCE=main.cpp dpkg.cpp systemtap.cpp cmdserver.cpp
SERVER=topdeb-server
CLIENTSOURCE=mainclient.cpp cmdclient.cpp
CLIENT=topdeb-client


CC=g++ -g

LIBS=-lboost_system -lboost_filesystem -lboost_program_options

CCXXFLAGS=-std=c++0x

all: $(SERVER) $(CLIENT)


$(SERVER): $(SERVERSOURCE)

	$(CC) $(CCXXFLAGS) $(SERVERSOURCE) $(LIBS) -o$(SERVER)

$(CLIENT): $(CLIENTSOURCE)

	$(CC) $(CCXXFLAGS) $(CLIENTSOURCE) $(LIBS) -o$(CLIENT)


clean:

	rm -f $(BINARY)
