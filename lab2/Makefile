CC=g++
CFLAGS=-std=c++11
LDFLAGS=-lGL -lGLU -lglut -I ../lib/freeglut-3.0.0/include/GL

all: bresenham task

bresenham:
	$(CC) $(CFLAGS) -o bresenham.o bresenham.cpp $(LDFLAGS)

task:
	$(CC) $(CFLAGS) -o task.o task.cpp $(LDFLAGS)

clean:
	rm *.o
