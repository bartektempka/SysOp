CC = gcc -Wall


all: static shared

static:
	$(CC) -c lib.c
	ar rcs lib.a lib.o

shared:
	$(CC) -c -fPIC lib.c
	$(CC) -shared -fPIC -o lib.so lib.o

clean:
	rm -f *.o