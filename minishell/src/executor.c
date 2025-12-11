// executor.c
// fichier contenant le necessaire pour executer les commandes pour le minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/executor.h"

const char* internal_cmds_list[] = {"cd", "pwd", "echo", "history", NULL};
void (*internal_cmds[])(char **) = {&handle_cd, &handle_pwd, &handle_echo, &handle_history, NULL};

int create_child_process(Command* command)
{
    int status;
    pid_t pid = fork();

    if (pid == 0)
    {
        exit(execvp(command->args[0], command->args));
    } else if (pid > 0) {
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return EXIT_FAILURE;
        }

    } else {
        perror("Fork failed");
        return EXIT_FAILURE;
    }
}

void execute_commands(Commands* commands)
{
    int last_status = 0;

    for (int i = 0; i < commands->command_count; i++) {
        if (strcmp(commands->commands[i].args[0], "exit") == 0) {
            handle_exit(commands);
        }
        last_status = execute_command(&commands->commands[i]);

        if (i < commands->command_count - 1 && commands->operators[i] != NULL) {
            if (strcmp(commands->operators[i], "&&") == 0 && last_status != 0) {
                break; // Stop execution on failure
            } else if (strcmp(commands->operators[i], "||") == 0 && last_status == 0) {
                break; // Stop execution on success
            }
        } 
    }
}

int execute_command(Command* command)
{
    if (command->args == NULL || command->args[0] == NULL) {
        return EXIT_FAILURE; 
    }

    // Check for internal commands, if found point to the corresponding function in internal_cmds[]
    for (int i = 0; internal_cmds_list[i] != NULL; i++) {
        if (strcmp(command->args[0], internal_cmds_list[i]) == 0) {
            internal_cmds[i](command->args);
            return EXIT_SUCCESS;
        }
    }

    int status = create_child_process(command);
    return status; 
}

 