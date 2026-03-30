#ifndef ENV_H
#define ENV_H

void env_set(const char* name, const char* value);
const char* env_get(const char* name);
void env_free(void);

#endif