#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>

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
void one_char_at_a_time(FILE * input, FILE * output,int size){
    char buffer;
    fseek(input,-1,SEEK_CUR);
    for(int i = 0; i < size; i++){
        fread(&buffer,sizeof(char),1,input);
        fwrite(&buffer,sizeof(char),1,output);
        fseek(input,-2,SEEK_CUR);
        //printf("char is: %c \n",buffer);
    }

    
}

void block(int size, FILE * input, FILE * output){
    int readSize = 1024;
    char buffer[readSize];
    char buffer2[readSize];

    while(size > 0){
        if(size < 1024){
            readSize = size;
            }
        fseek(input,-readSize,SEEK_CUR);
        fread(buffer,sizeof(char),readSize,input);
        for(int i = 0; i < readSize; i++){
            buffer2[i] = buffer[readSize - i - 1];
        }
        fwrite(buffer2,sizeof(char),readSize,output);
        size -= readSize;
        fseek(input,-readSize,SEEK_CUR);
    }

}
int main(int argc, char** argv) {
    FILE *inputFile = fopen(argv[1],"r");
    if(inputFile == NULL){
        printf("input file can't be opened");
        return 1;
    }
    fseek(inputFile,0L,SEEK_END);
    int size = ftell(inputFile);
    FILE *outputFile = fopen(argv[2],"w");
    if(outputFile == NULL){
        printf("output file can't be opened");
        return 1;
    }
    printf("Loading one char at a time: \n");
    startTimer();
    one_char_at_a_time(inputFile,outputFile,size);
    stopTimer();
    fseek(inputFile,0L,SEEK_END);
    outputFile = fopen(argv[2],"w");
    if(outputFile == NULL){
        printf("output file can't be opened");
        return 1;
    }
    printf("Loading blocks: \n");
    startTimer();
    block(size,inputFile,outputFile);
    stopTimer();
    return 0;
}