CC=gcc
CFLAGS=-I. -lpthread

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: Main.o
	$(CC) -o tutor Main.o $(CFLAGS)
