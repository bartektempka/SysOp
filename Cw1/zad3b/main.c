
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

#ifndef DLL
#include "lib.h"
#endif
#ifdef DLL
#include <dlfcn.h>
struct arr {
         int size;
         int used;
         char** data;
     };
#endif
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
char* getPath(char* path){
    char* letter = &path[strlen(path)-1];
    while (*letter != '/'){
        letter--;
    }
    letter++;
    *letter = '\0';
    return path;
}
int main(int argc, char *argv[]){
    char* path = getPath(argv[0]);
    #ifdef DLL
    void *handle = dlopen("./liblib.so", RTLD_LAZY);
    if(handle == NULL){
        fprintf(stderr, "%s\n", dlerror());
        printf("Cannot find library liblib.so. Terminating...");
        return -1;
    }


    struct arr * (*initialize)(int) = dlsym(handle, "initialize");   
    void (*wordCount)(struct arr*,char*) = dlsym(handle, "wordCount");
    char* (*getData)(struct arr*, int) = dlsym(handle, "getData");
    void (*deleteData)(struct arr*, int) = dlsym(handle, "deleteData");
    void (*deleteArr)(struct arr*) = dlsym(handle, "deleteArr");
    #endif

    struct arr* counter = NULL;
    char buffer[100];
    char completePath[100];
    
    while(fgets(buffer,100,stdin)!=NULL&& buffer[0] != '\n'){
        char * token = strtok(buffer, " ");
        token[strcspn(token, "\n")] = 0; 
        if(strcmp(token,"init")==0){
            int arg = (int) strtol(strtok(NULL, " "),(char **)NULL,10);
            startTimer();
            counter = initialize(arg);
            stopTimer();
        } else if (strcmp(token,"count") == 0){
            strcpy(completePath, "");
            strcat(completePath,path);
            strcat(completePath,strtok(NULL," "));
            printf("%s",completePath);
            startTimer();
            wordCount(counter,completePath);
            stopTimer();
        }else if(strcmp(token,"show") == 0){
            int arg = (int) strtol(strtok(NULL, " "),(char **)NULL,10);
            startTimer();
            printf("%s\n",getData(counter,arg));
            stopTimer();
        }else if(strcmp(token,"delete") == 0){
            if(strcmp(strtok(NULL," "),"index")==0){
                int arg = (int) strtol(strtok(NULL, " "),(char **)NULL,10);
                printf("deleted index: %d\n",arg);
                startTimer();
                deleteData(counter,arg);
                stopTimer();
            }
        }else if(strcmp(token,"destroy") == 0){
            printf("Freeing memory...\n");
            startTimer(); 
            deleteArr(counter);
            stopTimer();
        }
    }
    #ifdef DLL
        dlclose(handle);
    #endif
}
