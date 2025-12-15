#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "typedef.h"
#include "internal_commands.h"
#include "aliases.h"
#include "history.h"

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

/**
 * @brief Checks the status of background processes and cleans up finished ones.
 * @param bg_processes Pointer to the BackgroundProcess struct managing background jobs.
 */
void check_bg_processes(BackgroundProcess* bg_processes); 

/**
 * @brief Executes a series of piped commands.
 * @param commands Pointer to the Commands struct containing commands and operators.
 * @param start_index Index of the first command in the pipe sequence.
 * @param end_index Index of the last command in the pipe sequence.
 * @return Exit status of the last command in the pipe sequence.
 */
int execute_pipes(Commands* commands, int start_index, int end_index);

/**
 * @brief Sets up input and output redirections for a command.
 * @param command Pointer to the Command struct to set up redirections for.
 * @return 0 on success, -1 on failure.
 */
int setup_redirections(Command* command);

#endif