#ifndef INTERNAL_COMMANDS_H
#define INTERNAL_COMMANDS_H

#include "typedef.h"
#include "aliases.h"

extern char *work_dir;

/**
 * @brief Built-in cd command handler.
 * @param args Command arguments, handles correctly changing directories, and specific cases like "cd ~". 
 */
void handle_cd(char** args);

/**
 * @brief Built-in pwd command handler.
 * @param args voided   
 */
void handle_pwd(char** args);

/**
 * @brief Built-in echo command handler. 
 * @param args Command arguments to echo back to the user.
 */
void handle_echo(char** args);

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

/**
 * @brief Built-in exit command handler.
 * Exits the shell after freeing necessary resources.
 * @param commands Pointer to the Commands struct containing commands and operators.
 */
void handle_exit(Commands* commands);

/**
 * @brief Frees memory if the pointer is not NULL.
 * @param to_free Pointer to the memory to free.
 */
void free_if_needed(void* to_free);

/**
 * @brief Frees all allocated memory in the Commands struct.
 * @param commands Pointer to the Commands struct to free.
 */
void free_commands(Commands* commands);

#endif // INTERNAL_COMMANDS_H