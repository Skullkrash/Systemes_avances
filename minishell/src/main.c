// Projet de minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include <stdbool.h>
#include <unistd.h>

#include "../include/parser.h"

bool is_exiting = false;

// Variables de gestion de la ligne d'entrée
char *line = NULL;
size_t len = 0;

char** args = NULL;

void free_if_needed(void *to_free)
{
    if (to_free != NULL)
        free(to_free);
}

bool is_exit_command(const char *command)
{
    if (command == NULL)
        return false;

    if (strcmp(command, "exit") == 0)
    {
        printf("Exiting minishell...\n");
        return true;
    }

    return false;
}

int main(int argc, const char *argv[])
{
    while (!is_exiting)
    {
        printf("minishell> ");

        if (getline(&line, &len, stdin) != -1)
        {

            line[strcspn(line, "\n")] = 0;
            if (strlen(line) == 0)
                continue; // si la ligne est vide

            is_exiting = is_exit_command(line);

            args = parse_command(line);

            // executor simple 
            // TODO : executor.c avec gestion des erreurs et des pipes/redirections
            if (fork() == 0)
            {
                exit(execvp(args[0], args));
            }

            wait(NULL);
        }
        else
        {
            is_exiting = true;
        }
    }
    
    free_if_needed(line);
    free_if_needed(args);
    return 0;
}