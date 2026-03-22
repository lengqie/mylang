#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

static char* read_source(char* filename){
    FILE* f = fopen(filename, "rb");
    if(f == NULL){
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);
    return buffer;

}

int main(int argc, char *argv[])
{
    if(argc > 1){
        char* source = read_source(argv[1]);
        parser(source);
        free(source);
    } else {
        printf("Hello World");
    }
    return 0;
}