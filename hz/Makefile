LIB=libhzchip.a
CFLAGS:=-std=c99 -Wall -Wno-char-subscripts -O2 $(CFLAGS)

static:
	clang -c *.c $(CFLAGS)
	ar -rs $(LIB) *.o
