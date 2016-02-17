CC=gcc
CFLAGS=-I.
DEPS=FileUtils.h

LIBS=-lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Main: Main.o FileUtils.o
	$(CC) -o Assignment1 Main.o FileUtils.o $(CFLAGS) $(LIBS)
