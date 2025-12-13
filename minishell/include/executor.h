#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "typedef.h"
#include "internal_commands.h"

/**
 * @brief Executes a single command, checking for internal commands first.
 * @param command Pointer to the Command struct to execute.
 * @param is_background Boolean indicating if the command should run in the background.
 * @return Exit status of the command execution.
 */
int execute_command(Command* command, bool is_background);

/**
 * @brief Executes a series of commands with their associated operators.
 * @param commands Pointer to the Commands struct containing commands and operators.
 */
void execute_commands(Commands* commands, BackgroundProcess* bg_processes);

/**
 * @brief Creates a child process to execute a command.
 * @param command Pointer to the Command struct to execute.
 * @param is_background Boolean indicating if the command should run in the background.
 * @return Exit status of the command execution.
 */
int create_child_process(Command* command, bool is_background);

#endif