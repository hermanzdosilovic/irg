CC=g++
CFLAGS=-std=c++11
LDFLAGS=-lGL -lGLU -lglut -I ../lib/glm/

ODIR=obj
_OBJ=task.o bresenham.o polygon.o polyartist.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

DEPS=bresenham.h polygon.h polyartist.h
EXECUTABLE=task

DIRS=$(ODIR)

all: $(EXECUTABLE)

$(DIRS):
	mkdir -p $@

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

$(ODIR)/%.o: %.cpp $(DIRS) $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	rm $(OBJ) $(EXECUTABLE)
	rm -rf $(DIRS)
