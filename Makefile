LFLAGS=-lm -lepoxy -lGL -lSDL2
CFLAGS:=-std=c99 -Wall -Wno-char-subscripts $(CFLAGS)

all:
	mkdir -p bin/
	clang *.c $(CFLAGS) $(LFLAGS) -O2 -o bin/test

debug:
	mkdir -p bin/
	clang *.c -$(CFLAGS) DDEBUG -g $(LFLAGS) -o bin/test
