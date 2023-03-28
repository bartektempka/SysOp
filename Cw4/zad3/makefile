CC = gcc -Wall

main: clean
	$(CC) sender.c -o sender
	$(CC) catcher.c -o catcher
	$(CC) catcher2.c -o catcher2

clean:
	rm -f sender catcher