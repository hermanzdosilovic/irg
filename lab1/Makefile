CC=g++
CFLAGS=-std=c++11
LDFLAGS=-I ../lib/glm/

SOURCES=task1.cpp task2.cpp task3.cpp
EXECUTABLES=$(SOURCES:.cpp=)

all: $(EXECUTABLES)

$(EXECUTABLES): %: %.cpp
	$(CC) $(CFLAGS) -o $@ $@.cpp $(LDFLAGS)

clean:
	rm $(EXECUTABLES)
