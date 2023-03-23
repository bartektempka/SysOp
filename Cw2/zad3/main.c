#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

#include <stdlib.h>
int main(int argc, char** argv){
    long long cumSize = 0;
    DIR* workingDir = opendir("./");
    if (workingDir == NULL) {
        printf("Error opening directory");
        return 1;}
    struct dirent* dirEntry;
    struct stat statBuffer;
    while ((dirEntry = readdir(workingDir)) != NULL) {
        stat(dirEntry->d_name,&statBuffer);
        if(!S_ISDIR(statBuffer.st_mode)){
            cumSize += statBuffer.st_size;
        }
        printf("\n");
    }
    printf("cumulative size: %lld\n",cumSize);
}