CC = gcc -Wall -Werror

main:
	$(CC) main.c -o main
test: test1 test2 test3
test1:
	echo "./main .. #include" >> pomiar_zad_3.txt
	./main .. #include >> pomiar_zad_3.txt
	echo "\n" >> pomiar_zad_3.txt
test2:
	echo "./main . #include" >> pomiar_zad_3.txt
	./main . #include >> pomiar_zad_3.txt
	echo "\n" >> pomiar_zad_3.txt
test3:
	echo "./main ../.. #include" >> pomiar_zad_3.txt
	./main ../.. #include >> pomiar_zad_3.txt
	echo "\n" >> pomiar_zad_3.txt