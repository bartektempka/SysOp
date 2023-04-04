CC = gcc -Wall

main: clean
	$(CC) main.c -o main
	$(CC) integral.c -o integral
test: 
	rm -f raport_zad3.txt
	./main 0.0000000001 2 >> raport_zad3.txt
	./main 0.0000000001 4 >> raport_zad3.txt
	./main 0.0000000001 8 >> raport_zad3.txt
clean:
	rm -f main