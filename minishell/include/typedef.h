#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct Command {
  char *command;
  size_t length;
  char **args;
  int arg_count; // May not have a final use, don't forget to delete if unused 
} Command;

#endif // TYPEDEF_H