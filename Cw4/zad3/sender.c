#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void handler(int signal, siginfo_t* info, void* context){
    return;
}
int main(int argc, char **argv)
{
    int catcher_pid = atoi(argv[1]);
    union sigval val;
    sigset_t mask;
    struct sigaction act;
    act.sa_sigaction = handler;
    sigaction(SIGUSR1, &act, NULL);
    sigemptyset(&mask);
    for (int i = 2; i < argc; i++)
    {   
        
        val.sival_int = atoi(argv[i]);\
        sigqueue(catcher_pid,SIGUSR1,val);
        if(val.sival_int ==5){
            return 0;
        }
        sigsuspend(&mask);
    }
    
    
}