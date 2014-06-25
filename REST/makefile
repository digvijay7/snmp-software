CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lmicrohttpd -lpqxx -lpq
SOURCES=httphandler.cpp strutil.cpp api.cpp executor.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPENDENCIES=../lib/sha.cpp ../lib/token.cpp ../lib/checktoken.cpp
EXECUTABLE=rest-server
INCLUDES=-I../lib/

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(DEPENDENCIES) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
