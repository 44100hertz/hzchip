LFLAGS=-lm -lepoxy -lGL -lSDL2 -L./bin -lhzchip
CFLAGS:=-std=c99 -Wall -Wno-char-subscripts -O2 $(CFLAGS)

LIB=bin/libhzchip.a

all: lib main

main:
	clang main.c $(CFLAGS) $(LFLAGS) -o bin/test

lib:
	mkdir -p bin/
	cd hz/; \
		clang -c *.c $(CFLAGS)
	ar -rs $(LIB) hz/*.o

clean:
	rm hz/*.o
	rm bin/test $(LIB)
