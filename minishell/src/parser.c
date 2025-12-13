// parser.c
// fichier contenant le necessaire pour parser les commandes pour le minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/parser.h"

const char* OPERATORS[] = {"&&", "||", "&", "|", NULL}; // SUCCESS AND, FAIL OR, BACKGROUND, PIPE  

int detect_operator(const char* p, const char** op_found) {
    for (int i = 0; OPERATORS[i] != NULL; i++) {
        size_t len = strlen(OPERATORS[i]);
        if (strncmp(p, OPERATORS[i], len) == 0) {
            if (op_found) *op_found = OPERATORS[i];
            return (int)len;
        }
    }
    return 0;
}

void split_line(const char* line, Commands *commands)
{
    int index = 0;
    commands->command_count = 0;
    const char* start = line;
    const char* pos = line;

    free_commands(commands);

    while (*pos && index < MAX_COMMANDS) {
        const char* op = NULL;
        int op_len = detect_operator(pos, &op);
        if (op_len > 0) {
            size_t cmd_length = pos - start;
            commands->commands[index].command = strndup(start, cmd_length);
            commands->operators[index] = strdup(op);
            index++;
            pos += op_len;
            start = pos;
        } else {
            pos++;
        }
    }

    if (*start && index < MAX_COMMANDS) {
        while (*start == ' ' || *start == '\t') start++;
        const char* end = start + strlen(start);
        while (end > start && (*(end-1) == ' ' || *(end-1) == '\t')) end--;
        if (end > start) { 
            size_t cmd_length = end - start;
            commands->commands[index].command = strndup(start, cmd_length);
            commands->operators[index] = NULL;
            index++;
        }
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