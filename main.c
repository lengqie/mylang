#include <stdio.h>

static char* read_source(char* filename){
    return 0;
}
int main(int argc, char *argv[])
{
    if(argc > 1){
        char* source = read_source(argv[1]);
    }
    printf("Hello World");
    return 0;
}