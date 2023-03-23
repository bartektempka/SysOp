#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <errno.h>
char * prefix;
int openDir(char *path){
    DIR* workingDir = opendir(path);
    if (workingDir == NULL) {
        printf("Error opening directory %s\n",strerror( errno ));
        printf("path: %s\n",path);
        return 1;}
    pid_t child_pid;
    struct dirent* dirEntry;
    struct stat statBuffer;
    char filePath[PATH_MAX];
    
    while ((dirEntry = readdir(workingDir)) != NULL) {
        if(strcmp(dirEntry->d_name,".")==0 || strcmp(dirEntry->d_name,"..")==0){
            continue;
        }
        strcpy(filePath, path);
        strcat(filePath, "/");
        strcat(filePath, dirEntry->d_name);
        stat(filePath,&statBuffer);
        
        if(!S_ISDIR(statBuffer.st_mode)){
            FILE *file = fopen(filePath,"r");
            if(file == NULL){
                printf("Error opening file %s\n",strerror( errno ));
                return 1;
            }
            fseek(file,0L,SEEK_END);
            int inputSize = ftell(file);
            fseek(file,0,SEEK_SET);
            char * buffer = malloc((inputSize+1) * sizeof(char));
            if(fread(buffer, sizeof(char), inputSize, file) < inputSize){
                printf("Error reading file %s\n",strerror(errno));
            }
            if(strncmp(prefix, buffer, strlen(prefix)) == 0){
                printf("file path: %s \n pid: %d\n", filePath, getpid());
            }
            fclose(file);
            
        }
        else{
            child_pid = fork();
            if (child_pid == 0) {
                strcat(path, "/");
                strcat(path, dirEntry->d_name);
                openDir(path);
                exit(0);
            }
        }   
        strcpy(filePath, "");
    }
    return 0;
}
int main(int argc, char *argv[]){
    setbuf(stdout, NULL);
    char path[PATH_MAX] = "";
    strcat(path, argv[1]);
    prefix = argv[2];
    openDir(path);
    while(wait(NULL) > 0);
    return 0;


}
