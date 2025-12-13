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
    // Simple parser that splits command string into arguments based on spaces and handles quotes
    char* cmd_copy = strdup(command->command);
    char** args = (char**)malloc(sizeof(char*) * 64);
    
    int i = 0;
    bool in_quotes = false;
    bool in_double_quotes = false;
    char* cmd_parser = cmd_copy;
    char* arg_start = cmd_copy;
    int arg_length = 0;
    bool opening_quote_found = false;
    bool closing_quote_found = false;
    
    while (*cmd_parser) {
        if (*cmd_parser == '\'' && !in_double_quotes) {
            opening_quote_found = in_quotes == false ? true : false;
            closing_quote_found = in_quotes == true ? true : false;
            in_quotes = !in_quotes;
        } else if (*cmd_parser == '"' && !in_quotes) {
            opening_quote_found = in_double_quotes == false ? true : false;
            closing_quote_found = in_double_quotes == true ? true : false;
            in_double_quotes = !in_double_quotes;
        } else if (*cmd_parser == ' ' && !in_quotes && !in_double_quotes) {
            if (arg_length > 0) {
                args[i++] = strndup(arg_start, arg_length);
                arg_length = 0;
            }
            cmd_parser++;
            arg_start = cmd_parser;
            continue;
        }

        if (opening_quote_found) {
            opening_quote_found = !opening_quote_found;

            if (arg_length > 0) {
                args[i++] = strndup(arg_start, arg_length);
                arg_length = 0;
            }
            cmd_parser++;
            arg_start = cmd_parser;
            continue;
        }

        if (closing_quote_found) {
            closing_quote_found = !closing_quote_found;

            if (arg_length > 0) {
                char *arg = strndup(arg_start, arg_length);
                // Remove quotes from the argument
                for (int j = 0; arg[j]; j++) {
                    if (arg[j] == '\'' || arg[j] == '"') {
                        memmove(&arg[j], &arg[j + 1], strlen(arg) - j);
                        j--;
                    }
                }
                args[i++] = arg;
                arg_length = 0;
            }
            cmd_parser++;
            arg_start = cmd_parser;
            continue;
        }
        arg_length++;
        cmd_parser++;
    }
    
    if (arg_length > 0) {
        args[i++] = strndup(arg_start, arg_length);
    }
    
    args[i] = NULL;
    command->args = args;
    command->arg_count = i;

    free(cmd_copy);
}