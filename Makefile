all:
	mkdir -p bin/
	clang *.c -std=c99 -Wall -lm -lepoxy -lGL -lSDL2 -O2 -o bin/test
