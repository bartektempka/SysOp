#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void handler_NOCLDSTOP(int signum)
{
    printf("Recived signal SIGCHLD\n");
}
void flag_NOCLDSTOP()
{ // flag SA_NOCLDSTOP causes that SIGCHLD is not sent when child process is stopped
    struct sigaction act;
    act.sa_handler = handler_NOCLDSTOP;
    printf("No flag:\n");
    sigaction(SIGCHLD, &act, NULL);
    if (0 == fork())
    {
        printf("Killing child process\n");
        raise(SIGKILL);
        exit(0);
    }
    else
    {
        wait(0);
    }
    act.sa_flags = SA_NOCLDSTOP;
    printf("Flag SA_NOCLDSTOP:\n");
    if (0 == fork())
    {
        printf("Killing child process\n");
        raise(SIGKILL);
        exit(0);
    }
    else
    {
        wait(0);
    }
    printf("\n");
}
void handler_RESETHAND(int signum)
{
    printf("Recivied signal\n");
}
void flag_RESETHAND()
{ // flag SA_RESETHAND causes that signal handler is reset to default after first signal
    struct sigaction act;
    act.sa_handler = handler_RESETHAND;
    printf("No flag:\n");
    sigaction(SIGCHLD, &act, NULL);
    printf("rising signal\n");
    raise(SIGCHLD);
    printf("rising signal\n");
    raise(SIGCHLD);
    act.sa_flags = SA_RESETHAND;
    printf("Flag SA_RESETHAND:\n");
    sigaction(SIGCHLD, &act, NULL);
    printf("rising signal\n");
    raise(SIGCHLD);
    printf("rising signal\n");
    raise(SIGCHLD);
    printf("\n");
}
void info_handler(int signo, siginfo_t* info, void* context) {
    printf("Signal number: %d\n", info->si_signo);
    printf("PID: %d\n",  info->si_pid);
    printf("UID: %d\n",  info->si_uid);
    printf("POSIX timer ID: %d\n",  info->si_timerid);
    printf("Exit value / signal: %d\n",info->si_status);
    printf("Message: \"%s\"\n",(char*) info->si_value.sival_ptr);
}

void flag_SIGINFO()
{ // flag SA_SIGINFO causes that signal handler is called with 3 arguments
    struct sigaction act;
    act.sa_sigaction = info_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    printf("Flag SA_SIGINFO:\n");
    printf("Signal from main to main\n");
    char* mess1 = "Hello me.";
    union sigval val;
    val.sival_ptr = mess1;
    sigqueue(getpid(),SIGUSR1,val);
    if (0 == fork())
    {
        printf("Signal from child to main\n");
        char* mess2 = "Hello parent process.";
        val.sival_ptr = mess2;
        sigqueue(getppid(),SIGUSR1,val);
        exit(1);
    }
    else{
        wait(NULL);
    }
}
int main(int argc, char **argv)
{
    //setbuf(stdout, NULL);
    flag_RESETHAND();
    flag_NOCLDSTOP();
    flag_SIGINFO();
    return 0;
}