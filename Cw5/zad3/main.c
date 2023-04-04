#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/times.h>
double dx;
int n;
double lower_bound = 0.0;
double upper_bound = 1.0;
char arg3[100];

int main(int argc, char** argv){
    dx = strtod(argv[1],NULL);
    n = atoi(argv[2]);
    double curr_rectangle = lower_bound;
    double area;
    char command[1024];
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    mkfifo("pipe",0666);
    for(int i = 0;i <n;i++){
        if(0 == fork()){
            snprintf(command,1024,"./integral %.17f %d %.17f",dx,n,curr_rectangle);
            system(command);
            exit(0);
        }
        curr_rectangle += dx;
    }
    FILE* stream = fopen("pipe","r");
    area = 0;
    double res = 0;

    for(int i = 0;i <n;i++){
        fread(&area,sizeof(double),1,stream);
        res += area;
    }
    printf("------------------\n");

    remove("pipe");
        clock_gettime(CLOCK_REALTIME, &end);
    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("res: %f\ndx: %.17f\nn: %d\ntime: %f\n",res,dx,n,time_spent);
    printf("------------------\n");
}