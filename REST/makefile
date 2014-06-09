CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lmicrohttpd -lpq -lpqxx
SOURCES=httphandler.cpp strutil.cpp api.cpp executor.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPENDENCIES=../lib/sha.cpp
EXECUTABLE=rest-server
INCLUDES=-I../lib/

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(DEPENDENCIES) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
