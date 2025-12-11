// Projet de minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/typedef.h"
#include "../include/parser.h"
#include "../include/internal_commands.h"
#include "../include/executor.h"

// Structure de gestion de la ligne d'entrée
Command current_command = {NULL, 0, NULL, 0};

void free_if_needed(void *to_free)
{
    if (to_free != NULL) {
        free(to_free);
    }
}

void write_prompt(bool is_compact) 
{
    if (is_compact) {
        write(STDOUT_FILENO, "\033[1;32mminishell>\033[0m ", 20);
        return;
    } else {
        write(STDERR_FILENO, GREEN, 7);
        write(STDERR_FILENO, work_dir, strlen(work_dir));
        write(STDERR_FILENO, COLOR_RESET, 4);
        write(STDERR_FILENO, BLUE, 7);
        write(STDERR_FILENO, " minishell> ", 13);
        write(STDERR_FILENO, COLOR_RESET, 4);
        return;
    }
}

// Handling adding command to history using file descriptors
void add_to_history(Command command) {
    int history_descriptor;
    const char *home = getenv("HOME");
    size_t folder_size = strlen(home) + strlen("/logs") + 1;
    size_t history_size = strlen(home) + strlen("/logs/command_history_minishell") + 1;
    char history_path[history_size];
    strcpy(history_path, home);
    strcat(history_path, "/logs/command_history_minishell");
    
    // Create logs directory if it doesn't exist
    char logs_dir[folder_size];
    strcpy(logs_dir, home);
    strcat(logs_dir, "/logs");
    mkdir(logs_dir, 0755);

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
        write_prompt(0);

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

            execute_command(&current_command);

            add_to_history(current_command);

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