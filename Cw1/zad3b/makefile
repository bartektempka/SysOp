CC = gcc -Wall -Werror

all: static shared dynamic
#-O0,-O,-O1,-O2,-O3,-Os
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
test_all: test_static_compile test_shared_compile test_dynamic_compile
test_static_compile: 
	$(CC) -c lib.c -O0
	ar rcs library.a lib.o
	$(CC) main.c -o main_static -L. -lrary -O0
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_static) > result_static_O0.txt
	$(CC) -c lib.c -O
	ar rcs library.a lib.o
	$(CC) main.c -o main_static -L. -lrary -O
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_static) > result_static_O.txt
	$(CC) -c lib.c -O1
	ar rcs library.a lib.o
	$(CC) main.c -o main_static -L. -lrary -O1
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_static) > result_static_O1.txt
	$(CC) -c lib.c -O2
	ar rcs library.a lib.o
	$(CC) main.c -o main_static -L. -lrary -O2
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_static) > result_static_O2.txt
	$(CC) -c lib.c -O3
	ar rcs library.a lib.o
	$(CC) main.c -o main_static -L. -lrary -O3
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_static) > result_static_O3.txt
	$(CC) -c lib.c -Os
	ar rcs library.a lib.o
	$(CC) main.c -o main_static -L. -lrary -Os
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_static) > result_static_Os.txt
	make clean
test_dynamic_compile:
	$(CC) -c lib.c -O0
	$(CC) -shared -fPIC -o liblib.so lib.o -O0
	$(CC) main.c -o main_dynamic -ldl -D DLL -I. -O0
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_dynamic) > result_dynamic_O0.txt
	$(CC) -c lib.c -O
	$(CC) -shared -fPIC -o liblib.so lib.o -O
	$(CC) main.c -o main_dynamic -ldl -D DLL -I. -O
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_dynamic) > result_dynamic_O.txt
	$(CC) -c lib.c -O1
	$(CC) -shared -fPIC -o liblib.so lib.o -O1
	$(CC) main.c -o main_dynamic -ldl -D DLL -I. -O1
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_dynamic) > result_dynamic_O1.txt
	$(CC) -c lib.c -O2
	$(CC) -shared -fPIC -o liblib.so lib.o -O2
	$(CC) main.c -o main_dynamic -ldl -D DLL -I. -O2
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_dynamic) > result_dynamic_O2.txt
	$(CC) -c lib.c -O3
	$(CC) -shared -fPIC -o liblib.so lib.o -O3
	$(CC) main.c -o main_dynamic -ldl -D DLL -I. -O3
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_dynamic) > result_dynamic_O3.txt
	$(CC) -c lib.c -Os
	$(CC) -shared -fPIC -o liblib.so lib.o -Os
	$(CC) main.c -o main_dynamic -ldl -D DLL -I. -Os
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_dynamic) > result_dynamic_Os.txt

	make clean
test_shared_compile:
	$(CC) -c lib.c -O0
	$(CC) -shared -fPIC -o liblib.so lib.o -O0
	gcc main.c -Wall -o main_shared -L. -llib -I. -O0
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_shared) > result_shared_O0.txt
	$(CC) -c lib.c -O
	$(CC) -shared -fPIC -o liblib.so lib.o -O
	gcc main.c -Wall -o main_shared -L. -llib -I. -O
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_shared) > result_shared_O.txt
	$(CC) -c lib.c -O1
	$(CC) -shared -fPIC -o liblib.so lib.o -O1
	gcc main.c -Wall -o main_shared -L. -llib -I. -O1
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_shared) > result_shared_O1.txt
	$(CC) -c lib.c -O2
	$(CC) -shared -fPIC -o liblib.so lib.o -O2
	gcc main.c -Wall -o main_shared -L. -llib -I. -O2
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_shared) > result_shared_O2.txt
	$(CC) -c lib.c -O3
	$(CC) -shared -fPIC -o liblib.so lib.o -O3
	gcc main.c -Wall -o main_shared -L. -llib -I. -O3
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_shared) > result_shared_O3.txt
	$(CC) -c lib.c -Os
	$(CC) -shared -fPIC -o liblib.so lib.o -Os
	gcc main.c -Wall -o main_shared -L. -llib -I. -Os
	(echo "init 5\ncount main.c\ncount lib.c\ncount lib.h\nshow 0\nshow 1\nshow 2\ndelete index 0\ndelete index 1\ndelete index 2\ndestroy\n" | ./main_shared) > result_shared_Os.txt
	make clean
clean:
	rm -f *.o