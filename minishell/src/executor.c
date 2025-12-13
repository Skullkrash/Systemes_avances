// fichier contenant le necessaire pour executer les commandes pour le minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/executor.h"

const char* internal_cmds_list[] = {"cd", "pwd", "echo", "history", "alias", "unalias", NULL};
void (*internal_cmds[])(char **) = {&handle_cd, &handle_pwd, &handle_echo, &handle_history, &handle_alias, &handle_unalias, NULL};

int create_child_process(Command* command, bool is_background)
{
    int status;
    pid_t pid = fork();

    if (pid == 0)
    {
        if (is_background) {
            int in_fd = open("/dev/null", O_RDONLY);
            if (in_fd != -1) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
        }
        exit(execvp(command->args[0], command->args));
    } else if (pid > 0) {
        if (is_background) {
            return pid;
        } else {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else {
                return EXIT_FAILURE;
            }
        }

    } else {
        perror("Fork failed");
        return EXIT_FAILURE;
    }
}

void execute_commands(Commands* commands, BackgroundProcess* bg_processes)
{
    int last_status = 0;

    for (int i = 0; i < commands->command_count; i++) {
        if (strcmp(commands->commands[i].args[0], "exit") == 0) {
            handle_exit(commands);
        }

        bool is_background = false;

        if (commands->operators[i] && strcmp(commands->operators[i], "&") == 0) {
            is_background = true;
        }

        bool is_internal = false;
        for (int j = 0; internal_cmds_list[j] != NULL; j++) {
            if (strcmp(commands->commands[i].args[0], internal_cmds_list[j]) == 0) {
                is_internal = true;
                break;
            }
        }

        if (is_background && !is_internal) {
            int pid = create_child_process(&commands->commands[i], is_background);
            if (pid > 0 && bg_processes->count < MAX_BG_PROCESSES) {
                bg_processes->processes[bg_processes->count].pid = pid;
                bg_processes->processes[bg_processes->count].command = strdup(commands->commands[i].command);
                bg_processes->count++;
                printf("[%d] %d\n", bg_processes->count, pid);
                fflush(stdout);
            }
            last_status = 0;
        } else {
            last_status = execute_command(&commands->commands[i], is_background);
        }

        if (i < commands->command_count - 1 && commands->operators[i] != NULL) {
            if (strcmp(commands->operators[i], "&&") == 0 && last_status != 0) break; // Stop execution on failure
            if (strcmp(commands->operators[i], "||") == 0 && last_status == 0) break; // Stop execution on success
        } 
    }
}

int execute_command(Command* command, bool is_background)
{
    if (command->args == NULL || command->args[0] == NULL) {
        return EXIT_FAILURE; 
    }
    
    // Check for internal commands, if found point to the corresponding function in internal_cmds[]
    for (int i = 0; internal_cmds_list[i] != NULL; i++) {
        if (strcmp(command->args[0], internal_cmds_list[i]) == 0) {
            if(is_background) {
                printf("Internal command '%s' cannot be run in background.\n", command->args[0]);
                return EXIT_FAILURE;
            }
            internal_cmds[i](command->args);
            return EXIT_SUCCESS;
        }
    }

    int status = create_child_process(command, is_background);
    return status; 
}

void check_bg_processes(BackgroundProcess* bg_processes) 
{
    for (int i = 0; i < bg_processes->count; i++) {
        int status; 
        pid_t result = waitpid(bg_processes->processes[i].pid, &status, WNOHANG);

        if (result > 0) {
            printf("[%d]+  Done    %s\n", i+1, bg_processes->processes[i].command);
            free(bg_processes->processes[i].command);

            for (int j = i; j < bg_processes->count - 1; j++) {
                bg_processes->processes[j] = bg_processes->processes[j + 1];
            }
            bg_processes->count--;
            i--;
        }
    }
}
