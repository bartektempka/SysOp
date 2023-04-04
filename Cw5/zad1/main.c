#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char** argv){
    FILE* stream = NULL;
    FILE* fortune;
    char* buffer;

    stream = popen("cowsay","w");
    fortune = popen("fortune","r");
    fseek(fortune,0L,SEEK_END);
    int size = ftell(fortune);
    fseek(fortune,0,SEEK_SET);
    buffer =malloc((size+1) * sizeof(char));
    fputs(buffer, stream);
    pclose(fortune);
    pclose(stream);

    stream = popen("cowsay","r");
    fread(buffer,size,1,stream);
    printf("%s\n",buffer);
    pclose(stream);
}