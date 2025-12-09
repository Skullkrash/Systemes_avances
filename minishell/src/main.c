// Projet de minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/typedef.h"
#include "../include/parser.h"
#include "../include/internal_commands.h"
extern const char HISTORY_FILE[];

// Structure de gestion de la ligne d'entrée
Command current_command = {NULL, 0, NULL, 0};

void free_if_needed(void *to_free)
{
    if (to_free != NULL) {
        free(to_free);
    }
}

// Handling adding command to history using file descriptors
void add_to_history(Command* command) {
    const char* home = getenv("HOME");
    size_t path_size = strlen(home) + strlen(HISTORY_FILE) + 1; // accounting for '\0'
    char* history_path = malloc(path_size);

    snprintf(history_path, path_size, "%s%s", home, HISTORY_FILE);

    int history_descriptor = open(history_path, O_CREAT | O_APPEND | O_WRONLY, 0644);

    if (history_descriptor == -1) {
        perror("Error opening history file");
        free(history_path);
        return;
    }

    write(history_descriptor, command->command, strlen(command->command));
    write(history_descriptor, "\n", 1);

    if (close(history_descriptor) == -1) {
        perror("Error closing history file descriptor");
        exit(1);
    }
    free(history_path);
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