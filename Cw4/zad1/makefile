CC = gcc -Wall -Werror

main: clean
	$(CC) main.c -o main
	$(CC) exec_ignore.c -o exec_ignore
	$(CC) exec_mask.c -o exec_mask
	$(CC) exec_pending.c -o exec_pending
	
test : main test_child test_exec
	
test_child : main
	echo "ignore" >> test_zad1.txt
	./main ignore >> test_zad1.txt
	echo "\n" 	>> test_zad1.txt
	echo "handler" >> test_zad1.txt
	./main handler >> test_zad1.txt
	echo "\n" 	>> test_zad1.txt
	echo "mask" >> test_zad1.txt
	./main mask >> test_zad1.txt
	echo "\n" 	>> test_zad1.txt
	echo "pending" >> test_zad1.txt
	./main pending >> test_zad1.txt
	echo "\n" 	>> test_zad1.txt
test_exec: exec_ignore exec_mask exec_pending
	echo "execIgnore" >> test_zad1.txt
	./main execIgnore >> test_zad1.txt
	echo "\n" 	>> test_zad1.txt
	echo "execMask" >> test_zad1.txt
	./main execMask >> test_zad1.txt
	echo "\n" 	>> test_zad1.txt
	echo "execPending" >> test_zad1.txt
	./main execPending >> test_zad1.txt
	echo "\n" 	>> test_zad1.txt
	
clean:
	rm -f main exec_ignore exec_mask exec_pending