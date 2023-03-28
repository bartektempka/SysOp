#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
int main(int argc, char ** argv){
    raise(SIGUSR1);
    sigset_t waiting_mask;
    sigemptyset(&waiting_mask);
    sigpending(&waiting_mask);
    printf("Exec process, Signal blocked: %d\n",sigismember(&waiting_mask,SIGUSR1));
}