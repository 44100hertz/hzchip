LFLAGS=-lm -lepoxy -lGL -lSDL2 -L./hz -lhzchip
CFLAGS:=-std=c99 -Wall -O2 -Wno-missing-braces $(CFLAGS)

LIB=bin/libhzchip.a

all: lib main

main:
	mkdir -p bin/
	clang main.c $(CFLAGS) $(LFLAGS) -o bin/test

lib:
	make -C hz/

clean:
	rm hz/*.o
	rm bin/test $(LIB)
