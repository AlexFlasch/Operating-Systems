CC=gcc
CFLAGS=-I.
DEPS = CommandUtils.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: Main.o CommandUtils.o
	$(CC) -o shell Main.o CommandUtils.o $(CFLAGS)
