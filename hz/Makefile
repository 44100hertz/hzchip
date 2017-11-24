CC?=clang
LIB=libhzchip.a
CFLAGS:=-std=c99 -Wall -Wno-char-subscripts -O2 -lm $(CFLAGS)

static:
	$(CC) -c *.c $(CFLAGS)
	ar -rs $(LIB) *.o
