CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=testgen.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=TestGen

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf *.o TestGen