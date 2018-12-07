PROGRAM    = lonelyroom
CC	       = gcc
CFLAGS     = -g -Wall
LDLIBS     = -lGL -lGLU -lglut -lm

$(PROGRAM): lonelyroom.o image.o
	$(CC) $(CFLAGS) -o $(PROGRAM) image.o lonelyroom.o $(LDLIBS)

lonelyroom.o: sources/lonelyroom.c
	$(CC) $(CFLAGS) -c sources/lonelyroom.c $(LDLIBS)

image.o: sources/image.c
	$(CC) $(CFLAGS) -c sources/image.c $(LDLIBS)

.PHONY: clean 

clean: 
	-rm *.o $(PROGRAM) *core
