#ifndef ALIASES_H
#define ALIASES_H

#include "typedef.h"
#include <limits.h>

#define MAX_ALIASES 20

/**
 * @brief Handles the alias command and its various functionalities.
 * @param args The different arguments passed to the alias command.
 */
void handle_alias(char **args);

/**
 * @brief Handles the unalias command and its various functionalities.
 * @param args The different arguments passed to the unalias command (an alias name or -a).
 */
void handle_unalias(char **args);

/**
 * @brief Adds a new alias to the alias list.
 * @param new_alias The Alias struct containing the name and command of the new alias.
 */
void add_alias(Alias new_alias);

/**
 * @brief Removes an alias from the alias list from its name.
 * @param name The name of the alias to remove.
 */
void remove_alias(const char *name);

/**
 * @brief Checks if a given name corresponds to an existing alias.
 * @param name The name to check.
 * @return EXIT_SUCCESS if the name is an alias, EXIT_FAILURE otherwise.
 */
int is_alias(const char *name);

/**
 * @brief Lists all currently defined aliases.
 * Used when the alias command is called without arguments.
 */
void list_aliases();

/**
 * @brief Frees all allocated memory for aliases and resets the alias count.
 */
void free_aliases();

/**
 * @brief Retrieves the command associated with a given alias name.
 * @param name The name of the alias.
 * @return The command string if the alias exists, NULL otherwise.
 */
char* get_alias_command(const char *name);

#endif // ALIASES_H