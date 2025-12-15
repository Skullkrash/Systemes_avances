// Projet de minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/typedef.h"
#include "../include/parser.h"
#include "../include/internal_commands.h"
#include "../include/executor.h"
#include "../include/batch.h"

// Global variables
Commands parsed_commands;
BackgroundProcess bg_processes = {.count = 0};

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
        check_bg_processes(&bg_processes);
        write_prompt(0);
        char* line = NULL;
        size_t len = 0;

        if (getline(&line, &len, stdin) != -1)
        {
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) == 0){
                continue; // if empty line, re-prompt
            }

            split_line(line, &parsed_commands);

            for (int i = 0; i < parsed_commands.command_count; i++) {
                Command *cmd = &parsed_commands.commands[i];

                if (cmd->args == NULL || cmd->args[0] == NULL  || cmd->arg_count > 1) {
                    continue;
                }

                if (is_alias(cmd->args[0]) == 0) {
                    const char *alias_command = get_alias_command(cmd->args[0]);
                    if (alias_command == NULL) {
                        fprintf(stderr, "Error retrieving alias command for %s\n", cmd->args[0]);
                        return EXIT_FAILURE;
                    }

                    free_if_needed(cmd->command);
                    for (int i = 0; i < cmd->arg_count; i++)
                    {
                        free_if_needed(cmd->args[i]);
                    }
                    
                    cmd->command = strdup(alias_command);
                    parse_command(cmd);
                }
            }

            if (parsed_commands.command_count > 0) {
                execute_commands(&parsed_commands, &bg_processes);
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