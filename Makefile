SOURCE=main.cpp dpkg.cpp systemtap.cpp logger.cpp
BINARY=topdeb

CC=g++ -g

LIBS=-lboost_system -lboost_filesystem -lboost_program_options

CCXXFLAGS=-std=c++0x

all: $(BINARY)



$(BINARY): $(SOURCE)

	$(CC) $(CCXXFLAGS) $(SOURCE) $(LIBS) -o$(BINARY)

clean:

	rm -f $(BINARY)
