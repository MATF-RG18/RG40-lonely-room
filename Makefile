PROGRAM    = lonelyroom
CC         = gcc
CFLAGS     = -g -Wall
LDLIBS     = -lGL -lGLU -lglut

$(PROGRAM): lonelyroom.o
	$(CC) $(CFLAGS) -o $(PROGRAM) $^ $(LDLIBS) 

.PHONY: clean

clean:
	-rm *.o $(PROGRAM) *core
