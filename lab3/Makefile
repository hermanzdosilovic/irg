CC=g++
CFLAGS=-std=c++11
LDFLAGS=-lGL -lGLU -lglut -I ../lib/glm/

ODIR=obj
_OBJ=task.o bresenham.o polygon.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

DEPS=bresenham.h polygon.h
EXECUTABLE=task

DIRS=$(ODIR)

all: dirs task

dirs:
	mkdir -p $(DIRS)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	rm $(OBJ) $(EXECUTABLE)
	rm -rf $(DIRS)
