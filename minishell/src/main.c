// Projet de minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/typedef.h"
#include "../include/parser.h"

bool is_exiting = false;

// Structure de gestion de la ligne d'entrée
Command current_command = {NULL, 0, NULL, 0};

void free_if_needed(void *to_free)
{
    if (to_free != NULL) {
        free(to_free);
    }
}

bool is_exit_command(Command command)
{
    if (command.command == NULL) {
        return false;
    }

    if (strcmp(command.command, "exit") == 0)
    {
        printf("Exiting minishell...\n");
        return true;
    }

    return false;
}

// Handling adding command to history using file descriptors
void add_to_history(Command command) {
    int history_descriptor;
    history_descriptor = open("./log/command_history.txt", O_CREAT | O_APPEND | O_WRONLY, 0644);

    if (history_descriptor == -1) {
        perror("Error opening history file");
        exit(1);
    }

    size_t nbwrite = 0;
    char* text_to_write = malloc(strlen(command.command) + 2);
    text_to_write[0] = '\0';
    strcat(text_to_write, command.command);
    
    int i = 1;
    while (command.args[i] != NULL)
    {
        strcat(text_to_write, " ");
        strcat(text_to_write, command.args[i]);
        i++;
    }
    strcat(text_to_write, "\n");
    
    // Writing in file linked to history_descriptor
    nbwrite = write(history_descriptor, text_to_write, strlen(text_to_write));
    if (nbwrite != strlen(text_to_write)) {
        perror("Error writing to history file");
        exit(1);
    }

    if (close(history_descriptor) == -1) {
        perror("Error closing history file descriptor");
        exit(1);
    }
}

int main(int argc, const char *argv[])
{
    while (!is_exiting)
    {
        printf("minishell> ");

        if (getline(&current_command.command, &current_command.length, stdin) != -1)
        {
            current_command.command[strcspn(current_command.command, "\n")] = 0;
            if (strlen(current_command.command) == 0){
                continue; // si la ligne est vide
            }

            is_exiting = is_exit_command(current_command);

            parse_command(&current_command);

            add_to_history(current_command);

            // executor simple 
            // TODO : executor.c avec gestion des erreurs et des pipes/redirections
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