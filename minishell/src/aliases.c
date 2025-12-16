/**
 * @file aliases.c
 * @brief Implementation of alias handling for the minishell.
 * @author BRENNER Quentin, NEAGELY Jeannot
 * @date 2025-2026
 */

#include "../include/aliases.h"

/*
Concerning alias implementation :

- Can use the alias function to add a temporary alias (living along the minishell session)

Command possibilities :
- alias :
    - alias : list all aliases
    - alias name='command' : add alias
    - alias <alias_name> : print alias definition
- unalias :
    - unalias name : remove alias
    - unalias -a : remove all aliases
*/

/** The array of temporary aliases */
Alias aliases_list[MAX_ALIASES] = { {NULL, NULL} };

/** The current number of aliases */
int alias_count = 0;

void handle_alias(char **args) {    
    // If no additional arguments, list all aliases
    if (args[1] == NULL) {
        list_aliases();
        return;
    }

    // If only one argument, print its alias definition
    if (args[2] == NULL) {
        for (int i = 0; i < alias_count; i++) {
            if (strcmp(aliases_list[i].name, args[1]) == 0) {
                printf("alias %s='%s'\n", aliases_list[i].name, aliases_list[i].command);
                return;
            }
        }
        printf("alias: %s: not found\n", args[1]);
        return;
    }

    
    if (args[3] == NULL) {
        // Two arguments, meaning we need to add a new temporary alias
        Alias new_alias;
        new_alias.name = strtok(strdup(args[1]), "=");

        new_alias.command = malloc(strlen(args[2]) + 2);
        strcpy(new_alias.command, args[2]);
        
        add_alias(new_alias);

        return;
    }

    // Last possible case : If args has more than two elements, needs error handling
    if (args[3] != NULL) {
        printf("alias: too many arguments\n");
    }

    return;
}

void handle_unalias(char **args) {
    if (args[1] == NULL) {
        printf("unalias: missing argument\n");
        return;
    }

    if (strcmp(args[1], "-a") == 0) {
        free_aliases();
        return;
    }

    remove_alias(args[1]);
}

void add_alias(Alias new_alias) {
    aliases_list[alias_count] = new_alias;
    alias_count++;
}

void remove_alias(const char *name) {
    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(aliases_list[i].name, name) == 0) {
            free(aliases_list[i].name);
            free(aliases_list[i].command);

            // Shift remaining aliases
            for (int j = i; j < alias_count - 1; j++) {
                aliases_list[j] = aliases_list[j + 1];
            }
            alias_count--;
            return;
        }
    }
}

int is_alias(const char *name) {
    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(aliases_list[i].name, name) == 0) {
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;   
}

void list_aliases() {
    for (int i = 0; i < alias_count; i++) {
        printf("alias %s='%s'\n", aliases_list[i].name, aliases_list[i].command);
    }
}

void free_aliases() {
    for (int i = 0; i < alias_count; i++) {
        free(aliases_list[i].name);
        free(aliases_list[i].command);
    }
    alias_count = 0;
}

char* get_alias_command(const char *name) {
    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(aliases_list[i].name, name) == 0) {
            return aliases_list[i].command;
        }
    }
    return NULL;
}
