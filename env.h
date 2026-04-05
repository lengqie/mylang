#ifndef ENV_H
#define ENV_H

typedef enum{
    VAR_INT,        // integer
    VAR_STRING,     // string 
} VarType;

typedef struct{
    char* name;
    char* value;
    VarType type;
} Var;

void env_set(const char* name, const char* value,VarType type);
const Var* env_get(const char* name);
void env_free(void);

#endif