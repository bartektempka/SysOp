#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"



struct arr * initialize(int length){
    if (length <= 0){
        return NULL;
    }
    struct arr *res = malloc(sizeof(struct arr));
    res->size = length;
    res->used = 0;
    res->data = (char**)calloc(length, sizeof(char*));
    // res->data = memory;
    return res;
}

void wordCount(struct arr* x,char* file ){
    char command[100] = "mkdir -p \"tmp\" ; wc ";
    file[strcspn(file, "\n")] = 0;
    strcat(command,file);
    strcat(command," > tmp/tmp.txt");
    system(command);

    FILE *tmpFile = fopen("tmp/tmp.txt","r");
    if(tmpFile == NULL){
        printf("file can't be opened");
        system("rm -r tmp");
        return;
    }
    fseek(tmpFile,0L,SEEK_END);
    int size = ftell(tmpFile);
    fseek(tmpFile,0,SEEK_SET);

    x->data[x->used] =malloc((size+1) * sizeof(char));
    fread(x->data[x->used],size,1,tmpFile);
    x->data[x->used][size] = '\0';
    printf("%s\n",x->data[x->used]);
    x->used++;

    system("rm -r tmp"); //usuwanie buffora
}
char* getData(struct arr* x, int index){
    if (index >= x->used){
        printf("invalid index number ");
        return NULL;
    }
    return x->data[index];
}

void deleteData(struct arr* x, int index){
    if (index >= x->used){
        printf("invalid index number");
        return;
    }
    free(x->data[index]);
    x->data[index] = NULL;

}
void deleteArr(struct arr* x){
    for (int i = 0; i<x->size;i++){
        free(x->data[i]);
    }
    free(x->data);
    free(x);
}
