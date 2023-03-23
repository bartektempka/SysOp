CC = gcc -Wall -Werror

all: static shared dynamic

static:
	$(CC) -c lib.c
	ar rcs library.a lib.o
	$(CC) main.c -o main_static -L. -lrary
	make clean

shared:
	$(CC) -c lib.c
	$(CC) -shared -fPIC -o liblib.so lib.o
	gcc main.c -Wall -o main_shared -L. -llib -I.
	make clean
	
dynamic:
	$(CC) -c lib.c
	$(CC) -shared -fPIC -o liblib.so lib.o
	$(CC) main.c -o main_dynamic -ldl -D DLL -I.
	make clean
clean:
	rm -f *.o