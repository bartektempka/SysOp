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
test_all: test_static test_shared test_dynamic
test_static: static
	(echo "init 5\ncount main.c\ncount lib.c\ŚŚncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_static) > result_static.txt
test_dynamic: dynamic
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_dynamic) > result_dynamic.txt
test_shared: shared
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_shared) > result_shared.txt
		
clean:
	rm -f *.o