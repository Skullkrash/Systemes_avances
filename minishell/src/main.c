// Projet de minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/typedef.h"
#include "../include/parser.h"
#include "../include/internal_commands.h"

bool is_exiting = false;

// Structure de gestion de la ligne d'entrée
Command current_command = {NULL, 0, NULL, 0};

void free_if_needed(void *to_free)
{
    if (to_free != NULL)
        free(to_free);
}

int main(int argc, const char *argv[])
{
    work_dir = getcwd(NULL, 0);

    while (!is_exiting)
    {
        printf("minishell> ");

        if (getline(&current_command.command, &current_command.length, stdin) != -1)
        {
            current_command.command[strcspn(current_command.command, "\n")] = 0;
            if (strlen(current_command.command) == 0)
                continue; // si la ligne est vide

            parse_command(&current_command);

            is_exiting = is_exit_command(current_command);

            // executor simple 
            // TODO : executor.c avec gestion des erreurs et des pipes/redirections
            
            if (strcmp(current_command.command, "cd") == 0) {
                handle_cd(current_command.args);
                continue;
            }

            if (strcmp(current_command.command, "pwd") == 0) {
                handle_pwd();
                continue;
            }

            if (fork() == 0)
            {
                exit(execvp(current_command.command, current_command.args));
            }

            wait(NULL);
        }
        else
        {
            is_exiting = true;
        }
    }
    
    free_if_needed(current_command.command);
    free_if_needed(current_command.args);
    return 0;
}