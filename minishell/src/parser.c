// parser.c
// fichier contenant le necessaire pour parser les commandes pour le minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/parser.h"

const char* OPERATORS[] = {"&&", "||", "&", "|", NULL}; // SUCCESS AND, FAIL OR, BACKGROUND, PIPE  

void split_line(const char* line, Commands *commands)
{
    size_t num_operators = sizeof(OPERATORS) / sizeof(OPERATORS[0]) - 1;
    int index = 0;

    commands->command_count = 0; 
    const char* current = line;
    const char* last = line;

    while(*current && index < MAX_COMMANDS){
        const char* next_op = NULL;
        const char* found_op = NULL;

        // find the next operator in the line
        for(size_t i = 0; i < num_operators; i++) {
            const char* pos_op = strstr(current, OPERATORS[i]);
            if(pos_op && (!next_op || pos_op < next_op)) {
                next_op = pos_op;
                found_op = OPERATORS[i];
            }
        }

        // freeing if needed past command
        for (int i = 0; i < commands ->command_count; i++) {
            free(commands->commands[i].command);
            free(commands->commands[i].args);
            if (i < commands->command_count - 1) { // we only have command_count - 1 operators
                free(commands->operators[i]);
            }
        }
        
        // if an operator is found, split the command
        if(next_op) {
            size_t cmd_length = next_op - last;
            commands->commands[index].command = strndup(last, cmd_length);
            commands->operators[index] = strdup(found_op);
            index++; 
            current = next_op + strlen(found_op);
            last = current;
        } else {
            break; 
        }
    }

    if(*last && index < MAX_COMMANDS) {
        commands->commands[index].command = strdup(last);
        commands->operators[index] = NULL; 
        index++;    
    }

    for (int i = 0; i < index; i++) {
        parse_command(&commands->commands[i]);
    }
    commands->command_count = index;
}

void parse_command(Command* command) {
    char* cmd_copy = strdup(command->command);
    char** args = (char**)malloc(sizeof(char*) * 64);
    
    char* arg = strtok(cmd_copy, " ");
    int i = 0;
    
    while(arg != NULL) {
        args[i] = strdup(arg);
        i++;
        arg = strtok(NULL, " ");
    }
    
    args[i] = NULL;
    command->args = args;
    command->arg_count = i;
    
    free(cmd_copy);
}