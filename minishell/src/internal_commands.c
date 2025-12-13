#include "../include/internal_commands.h"

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
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
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

void handle_history(char** args) {
    (void)args; // To avoid unused parameter warning

    const char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "history: HOME path not set in env\n");
        return;
    }
    
    size_t history_size = strlen(home) + strlen("/minishell_logs/command_history") + 1;
    char history_path[history_size];
    strcpy(history_path, home);
    strcat(history_path, "/minishell_logs/command_history");
    
    if (access(history_path, F_OK) == -1) {
        fprintf(stderr, "history: log file not found\n");
        return;
    }

    int f1, val = 0;
    char buff;

    f1 = open(history_path, O_RDONLY);
    if (f1 == -1) {
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (val < lseek(f1, 0, SEEK_END)) {
        lseek(f1, val, SEEK_SET);
        read(f1, &buff, 1);
        write(1, &buff, 1);
        val++;
    }

    // Closing descriptor
    if (close(f1) == -1) {
        perror(strerror(errno));
        exit(1);
    }
}

// Handling adding command to history using file descriptors
void add_to_history(char* command_line) {
    int history_descriptor;
    const char *home = getenv("HOME");
    size_t folder_size = strlen(home) + strlen("/minishell_logs") + 1;
    size_t history_size = strlen(home) + strlen("/minishell_logs/command_history") + 1;
    char history_path[history_size];
    strcpy(history_path, home);
    strcat(history_path, "/minishell_logs/command_history");
    
    // Create logs directory if it doesn't exist
    char logs_dir[folder_size];
    strcpy(logs_dir, home);
    strcat(logs_dir, "/minishell_logs");
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