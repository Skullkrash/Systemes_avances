#ifndef HISTORY_H
#define HISTORY_H

#include "typedef.h"

/**
 * @brief Built-in history command handler.
 * Handles displaying the command history to the user.
 * @param args voided
 */
void handle_history(char** args);

/**
 * @brief Handles writing inputted commands in history file (~/minishell_logs/command_history).
 * @param command_line The entire command line that needs to be added in the history file.
 */
void add_to_history(char* command_line);

#endif // HISTORY_H