/**
 * @file internal_commands.c
 * @brief Implementation of internal commands for the minishell.
 * @author BRENNER Quentin, NEAGELY Jeannot
 * @date 2025-2026
 */

#include "../include/internal_commands.h"

/** The path to the current workdir */
char *work_dir;

void handle_exit(Commands* commands) {
    free_commands(commands);
    free_if_needed(work_dir);
    free_aliases();
    write(1, "Exiting minishell...\n", 21);
    exit(EXIT_SUCCESS);
}

void handle_pwd(char** args) {
    (void)args; // To avoid unused parameter warning
    write(STDOUT_FILENO, work_dir, strlen(work_dir));
    write(STDOUT_FILENO, "\n", 1);
}

void handle_cd(char** args) {
    if (args[1] == NULL || strcmp(args[1], "~") == 0) { // case "cd" or "cd ~"
        char* home = getenv("HOME");
        if (home != NULL) {
            args[1] = home;
        } else {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
    } else if (args[2] != NULL) {
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }

    if (chdir(args[1]) != 0) {
        perror("cd failed");
        return;
    }
    
    work_dir = getcwd(NULL, 0);
}

void handle_echo(char** args) {
    int i = 1;
    while (args[i] != NULL) {
        write(1, args[i], strlen(args[i]));
        if (args[i + 1] != NULL) {
            write(1, " ", 1);
        }
        i++;
    } 
    write(1, "\n", 1);
}

void free_if_needed(void *to_free)
{
    if (to_free != NULL) {
        free(to_free);
    }
}

void free_commands(Commands* commands)
{
    for (int i = 0; i < commands->command_count; i++) {
        free_if_needed(commands->commands[i].command);
        if (commands->commands[i].args != NULL) {
            for (int j = 0; j < commands->commands[i].arg_count; j++) {
                free_if_needed(commands->commands[i].args[j]);
            }
            free_if_needed(commands->commands[i].args);
        }
        
        if (i < commands->command_count - 1) {
            free_if_needed(commands->operators[i]);
        }
    }
}