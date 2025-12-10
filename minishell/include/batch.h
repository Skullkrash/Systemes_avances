#ifndef BATCH_H
#define BATCH_H

#include "typedef.h"

// Possible options to use when launching minishell
const char *minishell_options[] = {"-c", "--command", "--help", NULL};
extern Command batch_command = {NULL, 0, NULL, 0};

int handle_arguments(int argc, const char *argv[]);

#endif // BATCH_H