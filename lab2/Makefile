CC=g++
CFLAGS=-std=c++11
LDFLAGS=-lGL -lGLU -lglut -I ../lib/freeglut-3.0.0/include/GL

SOURCES=task.cpp bresenham.cpp
EXECUTABLES=$(SOURCES:.cpp=)

all: $(EXECUTABLES)

$(EXECUTABLES): %: %.cpp
	$(CC) $(CFLAGS) -o $@ $@.cpp $(LDFLAGS)

clean:
	rm $(EXECUTABLES)
