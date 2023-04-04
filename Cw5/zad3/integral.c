#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
char buffer[1024];
int main(int argc,char** argv){
    
    double dx = strtod(argv[1],NULL);
    int n = atoi(argv[2]);
    double start = strtod(argv[3],NULL);
    double width;
    double area = 0;
    while(start < 1.0){
        if(start + dx > 1.0){
            width = 1.0 - start;
        }else{
            width = dx;
        }
        area += width * (4/((start*start)+1));
        start += dx*n;
    }

    // asprintf(&buffer,"%f",area);
    // int size = snprintf(buffer, 1024, "%f\n", area);
    // int stream = open("pipe",O_WRONLY);
    // write(stream,buffer,size);
    // close(stream);
    FILE* stream = fopen("pipe","w");
    fwrite(&area,sizeof(double),1,stream);
}