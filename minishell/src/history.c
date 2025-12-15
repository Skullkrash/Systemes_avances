#include "../include/history.h"

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
