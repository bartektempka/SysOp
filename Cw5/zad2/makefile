CC = gcc -Wall

main: clean
	$(CC) main.c -o main
test: 
	rm -f raport_zad2.txt
	./main 0.0000000001 2 >> raport_zad2.txt
	./main 0.0000000001 4 >> raport_zad2.txt
	./main 0.0000000001 8 >> raport_zad2.txt

clean:
	rm -f main