LFLAGS=-lm -lepoxy -lGL -lSDL2 -L./hz -lhzchip
CFLAGS:=-std=c99 -Wall -Wno-char-subscripts -O2 $(CFLAGS)

all:
	mkdir -p bin/
	cd hz/; \
		clang -c *.c $(CFLAGS); \
		ar -rs libhzchip.a *.o
	clang main.c $(CFLAGS) $(LFLAGS) -o bin/test
