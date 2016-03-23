CC=g++
CFLAGS=-std=c++11
LDFLAGS=-lGL -lGLU -lglut -Ifreeglut-3.0.0/include/GL

all: bresenham

bresenham:
	$(CC) $(CFLAGS) -o bresenham.o bresenham.cpp $(LDFLAGS)

clean:
	rm *.o
