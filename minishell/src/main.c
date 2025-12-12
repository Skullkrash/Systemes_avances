// Projet de minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/typedef.h"
#include "../include/parser.h"
#include "../include/internal_commands.h"
#include "../include/executor.h"
#include "../include/batch.h"

// Structure de gestion de la ligne d'entrée
Commands parsed_commands;

void write_prompt(bool is_compact) 
{
    if (is_compact) {
        write(STDOUT_FILENO, BLUE "minishell> ", 18);
        write(STDOUT_FILENO, COLOR_RESET, 4);
    } else {
        write(STDOUT_FILENO, GREEN, 7);
        write(STDOUT_FILENO, work_dir, strlen(work_dir));
        write(STDOUT_FILENO, COLOR_RESET, 4);
        write(STDOUT_FILENO, BLUE" minishell> ", 19);
        write(STDOUT_FILENO, COLOR_RESET, 4);
    }
}

// Handling adding command to history using file descriptors
extern void add_to_history(char* command_line) {
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
    command_line[strcspn(command_line, "\n")] = '\n'; // ensure newline at end
    
    // Writing in file linked to history_descriptor
    nbwrite = write(history_descriptor, command_line, strlen(command_line));
    if (nbwrite != strlen(command_line)) {
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
    // If there are arguments, see if the option is -c, --command, or --help
    if (argc > 1) {
        if (handle_arguments(argc, argv) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        } else {
            return EXIT_SUCCESS;
        }
    }
    
    work_dir = getcwd(NULL, 0);

    while(true)
    {
        write_prompt(0);
        char* line = NULL;
        size_t len = 0;

        if (getline(&line, &len, stdin) != -1)
        {
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) == 0){
                continue; // si la ligne est vide
            }

            split_line(line, &parsed_commands);

            if (parsed_commands.command_count > 0) {
                execute_commands(&parsed_commands);
            }
            add_to_history(line);

            free(line);
            line = NULL;
            len = 0;
        } else {
            write(1,"\n",1);
            break;
        }
    }
}