#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_COMMANDS 3
#define MAX_BG_PROCESSES 10

#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define COLOR_RESET "\033[0m"

typedef struct Command {
  char *command;
  size_t length;
  char **args;
  int arg_count; // May not have a final use, don't forget to delete if unused 
} Command;

typedef struct Commands {
  Command commands[MAX_COMMANDS];
  char *operators[MAX_COMMANDS - 1];
  int command_count;
} Commands;

typedef struct Jobs {
  pid_t pid;
  char* command;
} Jobs;

typedef struct BackgroundProcess {
  int count;
  Jobs processes[MAX_BG_PROCESSES];
} BackgroundProcess;

#endif // TYPEDEF_H