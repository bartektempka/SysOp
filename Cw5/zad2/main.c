#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
double dx;
int n;
double lower_bound = 0.0;
double upper_bound = 1.0;

double calculate_integral(double start){
    double width;
    double area = 0;
    while(start < upper_bound){
        if(start + dx > upper_bound){
            width = upper_bound - start;
        }else{
            width = dx;
        }
        area += width * (4/((start*start)+1));
        start += dx*n;
    }
    return area;
}
int main(int argc, char** argv){
    dx = strtod(argv[1],NULL);
    n = atoi(argv[2]);
    double curr_rectangle = lower_bound;
    int streams[n][2];
    double area;

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    for(int i = 0;i <n;i++){
        pipe(streams[i]);
        if(0 == fork()){
            close(streams[i][0]);
            area = calculate_integral(curr_rectangle);
            write(streams[i][1],&area,sizeof(double));
            exit(0);
        }
        close(streams[i][1]);
        curr_rectangle += dx;
    }
    while(wait(NULL) > 0);
    area = 0;
    double res = 0;
    for(int i = 0;i <n;i++){
        read(streams[i][0],&area,sizeof(double));
        res += area;
    }
    printf("------------------\n");
    clock_gettime(CLOCK_REALTIME, &end);
    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("res: %f\ndx: %.17fl\nn: %d\ntime: %f\n",res,dx,n,time_spent);
    printf("------------------\n");

}