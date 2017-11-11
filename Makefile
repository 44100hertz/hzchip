all:
	mkdir -p bin/
	clang *.c -std=c99 -Wall -lSDL2 -lepoxy -o bin/test
