#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]){
    printf("%s ", argv[0]);
    fflush(stdout);
    if(argc != 2){
        printf("Niepoprawna liczba argumentow\n");
        return 1;
    }
    
    execl("/bin/ls", "ls", "-l",argv[1], NULL);

}