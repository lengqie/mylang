#include<stdio.h>
#include<string.h>
#include <stdlib.h>

#include "env.h"
#define MAX_VARS 256


typedef struct{
    char* name;
    char* value;
}Var;

static Var vars[MAX_VARS];
static int var_count = 0;
void env_set(const char* name, const char* value){
    if (var_count >= MAX_VARS){
        exit(1);
    }
    for (int i = 0; i < var_count; i++)
    {
        if (strcmp(vars[i].name,name) == 0){
            free(vars[i].value);
            vars[i].value =  strdup(value);
            return;
        }
    }
    vars[var_count].name =  strdup(name);
    vars[var_count].value = strdup(value);
    var_count++;
    
}
const char* env_get(const char* name){
    for (int i = 0; i < var_count; i++)
    {
        if (strcmp(vars[i].name,name) == 0){
            return vars[i].value;
        }
    }
    return NULL;
}
void env_free(void){
    for (int i = 0; i < var_count; i++)
    {
        free(vars[i].name);
        free(vars[i].value);
    }
    var_count = 0;
}