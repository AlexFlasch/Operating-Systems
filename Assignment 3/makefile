CC=gcc
CFLAGS=-I.

%.o: %.c
	$(CC) -c -w -o $@ $< $(CFLAGS)

all: Main.o
	$(CC) -o scheduling Main.o $(CFLAGS)
