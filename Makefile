LFLAGS=-lm -lepoxy -lGL -lSDL2
CFLAGS:=-std=c99 -Wall -Wno-char-subscripts $(CFLAGS)

all:
	mkdir -p bin/
	clang main.c hz/*.c $(CFLAGS) -O2 $(LFLAGS) -o bin/test

debug:
	mkdir -p bin/
	clang main.c hz/*.c $(CFLAGS) -DDEBUG $(LFLAGS) -o bin/test
