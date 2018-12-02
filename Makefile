PROGRAM    = lonelyroom
CC	       = gcc
CFLAGS     = -g -Wall
LDLIBS     = -lGL -lGLU -lglut -lm

$(PROGRAM): lonelyroom.o 
	$(CC) $(CFLAGS) -o $(PROGRAM) $^ $(LDLIBS)

lonelyroom.o: sources/lonelyroom.c
	$(CC) $(CFLAGS) -c sources/lonelyroom.c $(LDLIBS)

.PHONY: clean 

clean: 
	-rm *.o $(PROGRAM) *core
