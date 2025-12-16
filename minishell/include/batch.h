#ifndef BATCH_H
#define BATCH_H

#include "typedef.h"
#include "internal_commands.h"
#include "parser.h"
#include "executor.h"

#define MAX_ARGUMENTS 3

/**
 * @brief Handles command-line arguments for batch mode.
 * Possible options are:
 * - -c, --command <command>: Execute the specified command and exit.
 * - --help: Display help message and exit.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Returns EXIT_SUCCESS or EXIT_FAILURE.
 */
int handle_arguments(int argc, const char *argv[]);

#endif // BATCH_H