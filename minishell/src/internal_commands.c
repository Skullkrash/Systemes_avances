#include "../include/internal_commands.h"

const char HISTORY_FILE[] = "/.minishell_history";
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

void handle_pwd() {
    printf("%s\n", work_dir);
}

void handle_cd(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: expected argument\n");
        return;
    }

    if (strcmp(args[1], "~") == 0) {
        char* home = getenv("HOME");
        if (home != NULL) {
            args[1] = home;
        } else {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
    }

    if (chdir(args[1]) != 0) {
        perror("cd failed");
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

void handle_history() {
    const char* home = getenv("HOME");

    size_t history_size = strlen(home) + strlen(HISTORY_FILE) + 1;
    char history_path[history_size];
    snprintf(history_path, history_size, "%s%s", home, HISTORY_FILE);

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