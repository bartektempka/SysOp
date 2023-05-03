CC = gcc
CFLAGS = -Wall

main: clean 
	$(CC) $(CFLAGS) grid.c main.c -o main -lncursesw -lpthread -L/opt/homebrew/opt/ncurses/lib
clean:
	rm -f main
