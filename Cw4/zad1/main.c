#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
void hanler(int signum){
    printf("Odebrano sygnal SIGUSR1 w %d\n",getpid());
}
void create_child(){
    if(0 == fork()){
        raise(SIGUSR1);
        printf("Child process here still staying stong\n");
    }
}
int main(int argc, char** argv){
    if (strcmp(argv[1],"ignore") == 0){
        struct sigaction act;
        act.sa_handler = SIG_IGN;
        sigaction(SIGUSR1, &act, NULL);
        raise(SIGUSR1);
        create_child();
    }
    else if (strcmp(argv[1],"handler") == 0){
        struct sigaction act;
        act.sa_handler = hanler;
        sigaction(SIGUSR1, &act, NULL);
        raise(SIGUSR1);
        create_child();
    }
    else if(strcmp(argv[1],"mask") == 0){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        raise(SIGUSR1);
        create_child();
        sigset_t waiting_mask;
        sigemptyset(&waiting_mask);
        sigpending(&waiting_mask);
        printf("Signal blocked: %d PID: %d\n",sigismember(&waiting_mask,SIGUSR1),getpid());
    }
    else if(strcmp(argv[1],"pending")== 0){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        raise(SIGUSR1);
        if(0==fork()){
            sigset_t waiting_mask;
            sigemptyset(&waiting_mask);
            sigpending(&waiting_mask);
            printf("Child, Signal blocked: %d PID: %d\n",sigismember(&waiting_mask,SIGUSR1),getpid());
        }
        else{
            sigset_t waiting_mask;
            sigemptyset(&waiting_mask);
            sigpending(&waiting_mask);
            printf("Parent, Signal blocked: %d PID: %d\n",sigismember(&waiting_mask,SIGUSR1),getpid());
        }
    }
    else if(strcmp(argv[1],"execIgnore")==0){

        struct sigaction act;
        act.sa_handler = SIG_IGN;
        sigaction(SIGUSR1, &act, NULL);
        raise(SIGUSR1);
        if (0 == fork()){
            execl("./exec_ignore","./exec_ignore",NULL);
        }
    }
    else if(strcmp(argv[1],"execMask")==0){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        raise(SIGUSR1);
        execl("./exec_mask","./exec_mask",NULL);
    }
    else if(strcmp(argv[1],"execPending")==0){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        raise(SIGUSR1);
        execl("./exec_pending","./exec_pending",NULL);
        
    }
    else{
        printf("Wrong argument\n");
    }
}