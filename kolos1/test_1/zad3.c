#include "zad3.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/stat.h>
void readwrite(int pd, size_t block_size);

void createpipe(size_t block_size)
{
    /* Utwórz potok nienazwany */
    int stream[2];
    pipe(stream);


    /* Odkomentuj poniższe funkcje zamieniając ... na deskryptory potoku */
    check_pipe(stream);
    check_write(stream, block_size, readwrite);
}

void readwrite(int write_pd, size_t block_size)
{
    FILE* inputFile = fopen("unix.txt","r");
    //int fileOut = open("tmp.txt",O_CREAT | O_WRONLY);
    fseek(inputFile,0L,SEEK_END);
    long size = ftell(inputFile);
    fseek(inputFile,0L,SEEK_SET);
    char buffer[block_size];
    long readSize = block_size;
    while(size > 0){
        memset(buffer,0,strlen(buffer));
        if(size < block_size){
            readSize = size;
            }

        fread(buffer,sizeof(char),readSize,inputFile);
        write(write_pd,buffer,readSize*sizeof(char));
        //write(fileOut,buffer,readSize*sizeof(char));
        size -= readSize;
        //printf("%s",buffer);
    }
    //printf("\n");
    fclose(inputFile);
    close(write_pd);
    /* Otworz plik `unix.txt`, czytaj go po `block_size` bajtów
    i w takich `block_size` bajtowych kawałkach pisz do potoku `write_pd`.*/

    /* Zamknij plik */
}

int main()
{
    srand(42);
    size_t block_size = rand() % 128;
    createpipe(block_size);

    return 0;
}