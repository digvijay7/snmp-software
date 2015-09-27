CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=-lmicrohttpd -lpqxx -lpq -lpthread
SOURCES=httphandler.cpp strutil.cpp executor.cpp api.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPENDENCIES=../lib/sha.cpp ../lib/token.cpp ../lib/checktoken.cpp
EXECUTABLE=rest-server
INCLUDES=-I../lib/

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(DEPENDENCIES) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
