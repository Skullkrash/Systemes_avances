#include "../include/internal_commands.h"

char *work_dir;

bool is_exit_command(Command command)
{
    if (command.command == NULL)
        return false;

    if (strcmp(command.command, "exit") == 0)
    {
        printf("Exiting minishell...\n");
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

    if (chdir(args[1]) != 0) {
        perror("cd failed");
    }
    work_dir = getcwd(NULL, 0);
}