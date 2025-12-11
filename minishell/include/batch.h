#ifndef BATCH_H
#define BATCH_H

#include "typedef.h"
#include "internal_commands.h"
#include "parser.h"
#include "executor.h"

#define MAX_ARGUMENTS 3

int handle_arguments(int argc, const char *argv[]);

#endif // BATCH_H