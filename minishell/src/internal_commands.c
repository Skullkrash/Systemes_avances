#include "../include/internal_commands.h"

char *work_dir;

bool is_exit_command(Command* command)
{
    if (command -> command == NULL) {
        return false;
    }

    if (strcmp(command -> args[0], "exit") == 0) {
        return true;
    }

    return false;
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
    
    size_t history_size = strlen(home) + strlen("/logs/command_history_minishell") + 1;
    char history_path[history_size];
    strcpy(history_path, home);
    strcat(history_path, "/logs/command_history_minishell");
    
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