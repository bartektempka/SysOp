CC = gcc -Wall -Werror

main:
	$(CC) main.c -o main
test:
	mkdir -p test1
	mkdir -p test2
	mkdir -p test3
	number=1 ; while [[ $$number -le 10 ]] ; do \
        echo "aaaa" >> test2/$$number.txt ; \
        ((number = number + 1)) ; \
		done
	number=1 ; while [[ $$number -le 1000 ]] ; do \
        echo "aaaa" >> test3/$$number.txt ; \
        ((number = number + 1)) ; \
		done
	./main test1 >> pomiar_zad_2.txt
	./main test2 >> pomiar_zad_2.txt
	./main test3 >> pomiar_zad_2.txt
	rm -f -r test1 test2 test3