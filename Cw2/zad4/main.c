#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>

long long cumSize = 0;
int handleFile(const char *fpath, const struct stat *sb, int tflag){
    if(!S_ISDIR(sb->st_mode)){
        cumSize += sb->st_size;
        printf("%s %lld\n",fpath,sb->st_size);
    }
    return 0;
}
int main(int argc, char** argv){
    ftw(argv[1],&handleFile,1);
    printf("cumulative size: %lld\n",cumSize);
    return 0;
}