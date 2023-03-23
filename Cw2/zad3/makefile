CC = gcc -Wall -Werror

main:
	$(CC) main.c -o dirSize
test: main
	rm -f pomiar_zad_3.txt
	rm -f 1.txt
	rm -f 2.txt
	rm -f -r tmp
	echo "Test" >> pomiar_zad_3.txt
	number=1 ; while [[ $$number -le 500 ]] ; do \
        echo "h5JmEEG1x8RDCnraZUFX6TyhARsdRxLQSaNxjOHYotnVK9fzkvMwFm3I1gd1S4rHwFkHKUEuFigD0UDObLKGVr6q4TQskpIZbpAQ" > 1.txt ; \
        ((number = number + 1)) ; \
		done
	number=1 ; while [[ $$number -le 500 ]] ; do \
        echo "abcdefgh" > 2.txt ; \
        ((number = number + 1)) ; \
		done
	mkdir tmp 
	number=1 ; while [[ $$number -le 500 ]] ; do \
        echo "abcdefgh" > tmp/3.txt ; \
        ((number = number + 1)) ; \
		done
	
	wc -c * 1>tmp/tmp.txt 2> /dev/null || true
	./dirSize >> pomiar_zad_3.txt
	cat tmp/tmp.txt >> pomiar_zad_3.txt
	
	rm 1.txt 2.txt
	rm -r tmp
	