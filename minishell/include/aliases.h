#ifndef ALIASES_H
#define ALIASES_H

#include "typedef.h"
#include <linux/limits.h>
#include <linux/limits.h>

#define MAX_ALIASES 20
#define MAX_ALIAS_LINE_LENGTH 1024

void handle_alias(char **args); 
void handle_unalias(char **args);
void add_alias(Alias new_alias);
void remove_alias(const char *name);
int is_alias(const char *name);
void list_aliases();
void free_aliases();
char* get_alias_command(const char *name);

#endif // ALIASES_H