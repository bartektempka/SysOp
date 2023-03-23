#ifndef LIB_H
#define LIB_H
struct arr {
    int size;
    int used;
    char** data;
};
struct arr * initialize(int length);
void wordCount(struct arr* x,char* file );
char* getData(struct arr* x, int index);
void deleteData(struct arr* x, int index);
void deleteArr(struct arr* x);

#endif
