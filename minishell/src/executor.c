// executor.c
// fichier contenant le necessaire pour executer les commandes pour le minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/executor.h"

const char* internal_cmds_list[] = {"cd", "pwd", "echo", "history", NULL};
void (*internal_cmds[])(char **) = {&handle_cd, &handle_pwd, &handle_echo, &handle_history, NULL};

void create_child_process(Command* command)
{
    if (fork() == 0)
    {
        exit(execvp(command->args[0], command->args));
    }
}

void execute_command(Command* command)
{
    if (command->args == NULL || command->args[0] == NULL) {
        return;
    }

    // Check for internal commands, if found point to the corresponding function in internal_cmds[]
    for (int i = 0; internal_cmds_list[i] != NULL; i++) {
        if (strcmp(command->args[0], internal_cmds_list[i]) == 0) {
            internal_cmds[i](command->args);
            return;
        }
    }

    create_child_process(command);
}

