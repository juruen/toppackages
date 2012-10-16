SERVERSOURCE=server/main.cpp server/dpkg.cpp server/systemtap.cpp server/cmdserver.cpp
SERVER=topdeb-server
CLIENTSOURCE=client/main.cpp client/cmdclient.cpp
CLIENT=topdeb-client


CC=g++ -g

LIBS=-lboost_system -lboost_filesystem -lboost_program_options

CCXXFLAGS=-std=c++0x -Icommon

all: $(SERVER) $(CLIENT)


$(SERVER): $(SERVERSOURCE)

	$(CC) $(CCXXFLAGS) $(SERVERSOURCE) $(LIBS) -o$(SERVER)

$(CLIENT): $(CLIENTSOURCE)

	$(CC) $(CCXXFLAGS) $(CLIENTSOURCE) $(LIBS) -o$(CLIENT)


clean:

	rm -f $(CLIENT) $(SERVER)
