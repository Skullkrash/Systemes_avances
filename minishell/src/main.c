// Projet de minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/typedef.h"
#include "../include/parser.h"
#include "../include/internal_commands.h"

// Structure de gestion de la ligne d'entrée
Command current_command = {NULL, 0, NULL, 0};

void free_if_needed(void *to_free)
{
    if (to_free != NULL) {
        free(to_free);
    }
}

// Handling adding command to history using file descriptors
void add_to_history(Command command) {
    int history_descriptor;
    size_t history_size = strlen(work_dir) + strlen("/logs/command_history") + 1;
    char history_path[history_size];
    strcpy(history_path, work_dir);
    strcat(history_path, "/logs/command_history");

    history_descriptor = open(history_path, O_CREAT | O_APPEND | O_WRONLY, 0644);

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
    work_dir = getcwd(NULL, 0);

    while(true)
    {
        printf("%s minishell> ", work_dir);

        if (getline(&current_command.command, &current_command.length, stdin) != -1)
        {
            current_command.command[strcspn(current_command.command, "\n")] = 0;
            if (strlen(current_command.command) == 0){
                continue; // si la ligne est vide
            }

            parse_command(&current_command);

            if (is_exit_command(&current_command)) {
                break;
            }
            
            add_to_history(current_command);

            // executor simple 
            // TODO : executor.c avec gestion des erreurs et des pipes/redirections
            
            if (strcmp(current_command.args[0], "cd") == 0) {
                handle_cd(current_command.args);
                continue;
            }

            if (strcmp(current_command.args[0], "pwd") == 0) {
                handle_pwd();
                continue;
            }

            if (strcmp(current_command.args[0], "echo") == 0) {
                handle_echo(current_command.args);
                continue;
            }
            
            if (strcmp(current_command.args[0], "history") == 0) {
                handle_history();
                continue;
            }

            if (fork() == 0)
            {
                exit(execvp(current_command.args[0], current_command.args));
            }

            wait(NULL);
        } else {
            write(1,"\n",1);
            break;
        }
    }
    
    free_if_needed(current_command.command);
    free_if_needed(current_command.args);
    free_if_needed(work_dir);

    write(1, "Exiting minishell...\n", 21);
    return EXIT_SUCCESS;
}