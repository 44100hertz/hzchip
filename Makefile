LFLAGS:=-lm -lepoxy -lGL -lSDL2$(CFLAGS)

all:
	mkdir -p bin/
	clang *.c -std=c99 -Wall $(LFLAGS) -O2 -o bin/test

debug:
	mkdir -p bin/
	clang *.c -std=c99 -DDEBUG -Wall -g $(LFLAGS) -o bin/test
