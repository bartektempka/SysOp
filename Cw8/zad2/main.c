#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include "grid.h"
#include <pthread.h>
#include <signal.h>
#include <math.h>

char *foreground;
char *background;
char* tmp;
pthread_t* threads;
int** to_free;
int cells_per_thread;
int num_of_threads;
void handle_sig(int signum){
    if (signum == SIGSTOP){
        for(int i = 0; i < num_of_threads; i++){
            free(to_free[i]);
        }
        destroy_grid(foreground);
        destroy_grid(background);
        free(threads);
        free(to_free);
        endwin();
        exit(0);
    }
    return;
}
void* cell_life(void* index_void){
    int index = *((int*)index_void);
    int row;
    int col;
    for (int i = 0; i < cells_per_thread; i++){
            foreground[index+i] = rand()%2;
        }
    
    while(1){
        pause();
        for (int i = 0; i < cells_per_thread; i++){
            row = (index + i) / grid_width;
            col = (index + i) % grid_width;
            background[index+i] = is_alive(row,col,foreground);
        }
        
    }
}

int main(int argc,char** argv){
    num_of_threads = atoi(argv[1]);
    cells_per_thread = (int)ceil(grid_height * grid_width / (float)num_of_threads);

    srand(time(NULL));
	setlocale(LC_CTYPE, "");
    signal(SIGUSR1,handle_sig);
	initscr(); // Start curses mode
    threads = malloc(sizeof(pthread_t) * num_of_threads);
    to_free = malloc(sizeof(int*) * num_of_threads);
	foreground = create_grid();
	background = create_grid();
    int start_index = 0;
    for (int i = 0; i < num_of_threads ; i++){
        to_free[i] = malloc(sizeof(int));
        *(to_free[i]) = start_index;
        pthread_create(&threads[i],NULL,cell_life,to_free[i]);
        start_index += cells_per_thread;
    }
    while(1){
        draw_grid(foreground);
        for (int i = 0; i < num_of_threads; i++){
            pthread_kill(threads[i],SIGUSR1);
        }
		usleep(500 * 1000);
        tmp = foreground;
        foreground = background;
        background = tmp;
    }
    
    return 0;
}
