CC = gcc -Wall -Werror

main:
	$(CC) main.c -o reverse
test: main
	rm -f pomiar_zad_2.txt
	rm -f test.txt
	echo "Test 500000 linijek * 100 znaków" >> pomiar_zad_2.txt
	number=1 ; while [[ $$number -le 500000 ]] ; do \
        echo "h5JmEEG1x8RDCnraZUFX6TyhARsdRxLQSaNxjOHYotnVK9fzkvMwFm3I1gd1S4rHwFkHKUEuFigD0UDObLKGVr6q4TQskpIZbpAQ" >> test.txt ; \
        ((number = number + 1)) ; \
		done
	./reverse test.txt output.txt >> pomiar_zad_2.txt
	./reverse test.txt tmp1 ; ./reverse tmp1 tmp2 ; diff -s tmp2 test.txt >> pomiar_zad_2.txt
	#rm tmp1 tmp2 output.txt test.txt
	