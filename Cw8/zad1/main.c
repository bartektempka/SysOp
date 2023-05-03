#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include "grid.h"
#include <pthread.h>
#include <signal.h>

char *foreground;
char *background;
char* tmp;
pthread_t threads[grid_width * grid_height];
int* to_free[grid_width * grid_height];
void handle_sig(int signum){
    if (signum == SIGSTOP){
        for(int i = 0; i < grid_width * grid_height; i++){
            free(to_free[i]);
        }
        destroy_grid(foreground);
        destroy_grid(background);
        endwin();
        exit(0);
    }
    return;
}
void* cell_life(void* index_void){
    int index = *((int*)index_void);
    int row = index / grid_width;
    int col = index % grid_width;
    foreground[index] = rand()%2;
    while(1){
        pause();
        background[index] = is_alive(row,col,foreground);
    }
}

int main(int argc,char** argv){
    srand(time(NULL));
	setlocale(LC_CTYPE, "");
    signal(SIGUSR1,handle_sig);
	initscr(); // Start curses mode

	foreground = create_grid();
	background = create_grid();
    
    for (int i = 0; i < grid_height * grid_width; i++){
        foreground[i] = 0;
        to_free[i] = malloc(sizeof(int));
        *(to_free[i]) = i;
        int *index = malloc(sizeof(int));
        *index = i;
        pthread_create(&threads[i],NULL,cell_life,to_free[i]);
    }
    while(1){
        draw_grid(foreground);
        for (int i = 0; i < grid_height *grid_width; i++){
            pthread_kill(threads[i],SIGUSR1);
        }
		usleep(500 * 1000);
        tmp = foreground;
        foreground = background;
        background = tmp;
    }
    
    return 0;
}
