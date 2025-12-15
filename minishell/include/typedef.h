#ifndef TYPEDEF_H
#define TYPEDEF_H

/**
 * \mainpage Minishell
 *
 * \section intro_sec Introduction
 * Welcome to the documentation of the Minishell project for EICNAM 2025.
 *
 * \section features_sec Features
 * - Command parsing and execution
 * - Alias management
 * - Batch mode
 * - Command history
 * - Internal commands (cd, pwd, echo, etc.)
 *
 * \section usage_sec Usage
 * See the README for usage instructions.
 */

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

/** Structure to hold a single command and its details */ 
typedef struct Command {
  char*   command;                    // Original command string
  size_t  length;                     // Length of the command string
  char**  args;                       // Argument list
  int     arg_count;                  // Number of arguments related to the command
  char*   input_redirect;             // file for <
  char*   output_redirect;            // file for > or >>
  char*   heredoc_delimiter;          // delimiter for <<
  bool    append_output;              // true for >>, false for >
} Command;

/** Structure to hold multiple commands and their operators */
typedef struct Commands {
  Command commands[MAX_COMMANDS];         // Array of parsed commands
  char*   operators[MAX_COMMANDS];        // Operators between commands
  int     command_count;                  // Number of commands parsed
} Commands;

/** Structure to hold background job details */
typedef struct Jobs {
  pid_t   pid;                // Process ID
  char*   command;            // Command string
} Jobs;

/** Structure to manage background processes */
typedef struct BackgroundProcess {
  int     count;                              // Number of background processes
  Jobs    processes[MAX_BG_PROCESSES];        // Array of background processes
} BackgroundProcess;

typedef struct Alias {
  char*   name;             // Alias name
  char*   command;          // Command string
} Alias;

#endif // TYPEDEF_H