#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

time_t raw_time;
struct tm* time_info;
int num_of_requests = 0;
int sender_pid = 0;
int request_type = 0;
void handler(int signo, siginfo_t* info, void* context) {
    num_of_requests++;
    if(request_type == 4){
        kill(sender_pid,SIGUSR1);
    }
    request_type = info->si_value.sival_int;
    sender_pid = info->si_pid;
}

int main(int argc, char** argv){
    
    printf("catcher pid: %d\n",getpid());
    struct sigaction act;
    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO | SA_NODEFER;
    sigaction(SIGUSR1, &act, NULL);
    while(1){
        switch (request_type)
        {
        case 1:
            for (int i = 1; i <= 100; i++)
            printf("%i ", i);

            printf("\n");
            request_type = 0;
            kill(sender_pid,SIGUSR1);
            break;
        
        case 2:
            
            time(&raw_time);
            time_info = localtime(&raw_time);
            printf("Current time: %s", asctime(time_info));
            request_type = 0;
            kill(sender_pid,SIGUSR1);
            break;
        
        case 3:
            printf("Number of request since startup: %d\n",num_of_requests);
            request_type = 0;
            kill(sender_pid,SIGUSR1);
            break;
        
        case 4:
            time(&raw_time);
            time_info = localtime(&raw_time);
            printf("Current time: %s\n", asctime(time_info));  
            sleep(1);
            break;
        case 5:
            kill(sender_pid,SIGUSR1);
            exit(0);
            break;
        
        default:
            break;
    }
    
    }
    
    return 0;
}