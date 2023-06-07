CC = gcc -Wall

main: clean
	$(CC) server.c -o server -lpthread
	$(CC) client.c -o client -lpthread
clean:
	rm -f server client