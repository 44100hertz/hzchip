all:
	mkdir -p bin/
	clang *.c -std=c99 -Wall -lm -lGL -lSDL2 -o bin/test
