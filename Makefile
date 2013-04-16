CC=c++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp instDecoder.cpp instSimulator.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=MIPSsim

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf *.o MIPSsim