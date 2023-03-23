#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Niepoprawna liczba argumentow\n");
        return 1;
    }
    int x;
    int arg1 = atoi(argv[1]);
    for (int i = 0; i < arg1; i++) {
        x = fork();
        if (x == 0) {
            printf("Proces potomny %d, PID: %d, PPID: %d\n", i, getpid(), getppid());
            exit(0);
        }
        wait(NULL);
    }
    
    printf("%d",arg1);
    return 0;
}