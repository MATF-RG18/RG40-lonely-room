PROGRAM    = lonelyroom
CC	   = gcc
CFLAGS     = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDLIBS     = -lGL -lGLU -lglut -lm

$(PROGRAM): lonelyroom.o image.o ortho.o axes.o light.o 
	$(CC) $(CFLAGS) -o $(PROGRAM) image.o lonelyroom.o ortho.o axes.o light.o $(LDLIBS)

lonelyroom.o: sources/lonelyroom.c sources/lonelyroom.h
	$(CC) $(CFLAGS) -c sources/lonelyroom.c -o lonelyroom.o $(LDLIBS)

image.o: sources/image.c
	$(CC) $(CFLAGS) -c sources/image.c $(LDLIBS)

ortho.o: sources/ortho.c sources/ortho.h
	$(CC) $(CFLAGS) -c sources/ortho.c -o ortho.o $(LDLIBS)

axes.o: sources/axes.c sources/axes.h
	$(CC) $(CFLAGS) -c sources/axes.c -o axes.o $(LDLIBS)

light.o: sources/light.c sources/light.h
	$(CC) $(CFLAGS) -c sources/light.c -o light.o $(LDLIBS)
	
	
.PHONY: clean 

clean: 
	-rm *.o $(PROGRAM) *core
