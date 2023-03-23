#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>

#include <unistd.h>
#include <fcntl.h>
// #include <sys/types.h>
#include <sys/stat.h>

#include<errno.h>
struct tms start;
struct tms end;
clock_t startTime;
clock_t endTime;

void startTimer(){
    startTime = times(&start);
}
void stopTimer() {
    endTime = times(&end);
    double real_time = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    double user_time =
        (double)(end.tms_utime - start.tms_utime) / CLOCKS_PER_SEC
         +
        (double)(end.tms_cutime - start.tms_cutime) / CLOCKS_PER_SEC;
    double system_time =
        (double)(end.tms_stime - start.tms_stime) / CLOCKS_PER_SEC
         +
        (double)(end.tms_cstime - start.tms_cstime) / CLOCKS_PER_SEC;
        printf("Real time: %f, User time: %f, System time: %f\n",real_time,user_time,system_time);
}

void lib(char* input, char* output,char a,char b){
    FILE *inputFile = fopen(input,"r");
    if(inputFile == NULL){
        printf("input file can't be opened (lib)");
        return;
    }
    fseek(inputFile,0L,SEEK_END);
    int inputSize = ftell(inputFile);
    fseek(inputFile,0,SEEK_SET);
    char * buffer = malloc((inputSize+1) * sizeof(char));
    FILE *outputFile = fopen(output,"w");
    if(outputFile == NULL){
        printf("output file can't be opened (lib)");
        return;
    }
    fread(buffer,sizeof(char),inputSize,inputFile);
    for(int i = 0; i < inputSize; i++){
        if(buffer[i] == a ){
            buffer[i] = b;
        }
    }
    fwrite(buffer,sizeof(char),inputSize,outputFile);
}
void sys(char* input, char* output,char a,char b){
    int inputDescriptor = open(input,O_RDONLY);
    if(inputDescriptor == -1){
        printf("input file can't be opened (sys)\n");
        return;
    }
    int outputDescriptor = open(output,O_CREAT | O_WRONLY);
    if(outputDescriptor == -1){
        printf("output file can't be opened (sys)\n");
        return;
    }
    struct stat statBuffer;
    fstat(inputDescriptor,&statBuffer);
    char * buffer = malloc((statBuffer.st_size+1) * sizeof(char));
    read(inputDescriptor,buffer,statBuffer.st_size);
    for(int i = 0; i < statBuffer.st_size; i++){
        if(buffer[i] == a ){
            buffer[i] = b;
        }
    }
    write(outputDescriptor,buffer,statBuffer.st_size);
}

int main(int argc, char *argv[]) {
    if(strlen(argv[1]) > 1 || strlen(argv[2]) > 1){
        printf("argument 1 or 2 is not a char (too long)");
        return 1;
    }
    printf("LIB TIMES:\n");
    startTimer();
    lib(argv[3],argv[4],argv[1][0],argv[2][0]);
    stopTimer();
    printf("SYS TIMES:\n");
    startTimer();
    sys(argv[3],argv[4],argv[1][0],argv[2][0]);
    stopTimer();



   return 0;
}