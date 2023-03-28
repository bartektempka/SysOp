#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
int main(int argc, char ** argv){
    raise(SIGUSR1);
    printf("Exec process, Still alive, still kicking");
}