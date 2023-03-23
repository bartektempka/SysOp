CC = gcc -Wall -Werror

main:
	$(CC) main.c -o dirSize
test: main
	rm -f pomiar_zad_4.txt
	rm -f 1.txt
	rm -f 2.txt
	rm -f -r tmp
	echo "Test" >> pomiar_zad_4.txt
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
	./dirSize ./ >> pomiar_zad_4.txt
	find ./ | xargs wc -c >> pomiar_zad_4.txt || true
	echo "diffrence is due to saving result of ./dirSize in file" >> pomiar_zad_4.txt


	rm 1.txt 2.txt
	rm -r tmp