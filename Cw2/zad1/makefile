CC = gcc -Wall -Werror

main:
	$(CC) main.c -o zamien
test: main
	rm -f pomiar_zad_1.txt
	rm -f test.txt
	echo "Test 500000 linijek * 100 znaków \"a\" na \"x\"" >> pomiar_zad_1.txt
	number=1 ; while [[ $$number -le 500000 ]] ; do \
        echo "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" >> test.txt ; \
        ((number = number + 1)) ; \
		done
	./zamien a x test.txt o.txt >> pomiar_zad_1.txt
	tr a x <test.txt> tmp
	diff -s tmp o.txt >> pomiar_zad_1.txt
	#rm o.txt tmp test.txt
	