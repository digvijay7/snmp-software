CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lboost_system -lboost_thread -lpthread -lpq -lpqxx
SOURCES=serv.cpp Parse.cpp Log.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPENDENCIES=../lib/sha.cpp
EXECUTABLE=server
INCLUDES=-I../lib/
all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(DEPENDENCIES) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
