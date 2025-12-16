/**
 * @file parser.c
 * @brief Implementation of the command line parser for the minishell.
 * @author BRENNER Quentin, NEAGELY Jeannot
 * @date 2025-2026
 */

#include "../include/parser.h"

const char* OPERATORS[] = {"&&", "||", "&", "|", ";", NULL}; // SUCCESS AND, FAIL OR, BACKGROUND, PIPE, COMMAND_SEPARATOR
const char* REDIRECTORS[] = {"<<", ">>", "<", ">", NULL}; // HEREDOC, APPEND, INPUT, OUTPUT

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

char* trim_whitespace(const char* line) {
    if (line == NULL) return NULL;

    while (*line == ' ' || *line == '\t') line++; // Trim leading whitespace
    if (*line == '\0') return strdup("");

    const char* end = line + strlen(line) - 1;
    while (end > line && (*end == ' ' || *end == '\t')) end--; // Trim trailing whitespace
    
    size_t length = end - line + 1;
    char* trimmed = strndup(line, length);
    return trimmed;
}

void split_line(const char* line, Commands *commands)
{
    free_commands(commands); 
    
    int index = 0;
    commands->command_count = 0;
    const char* start = line;
    const char* pos = line;

    while(*start == ' ' || *start == '\t') {
        start++;
        pos++;
    }

    while (*pos && index < MAX_COMMANDS) { // parse until max commands
        const char* op = NULL;
        int op_len = detect_operator(pos, &op);
        if (op_len > 0) {
            size_t cmd_length = pos - start;
            char* cmd = strndup(start, cmd_length);
            commands->commands[index].command = trim_whitespace(cmd);
            commands->operators[index] = strdup(op);
            free(cmd);

            index++;
            pos += op_len;
            start = pos;

            while (*start == ' ' || *start == '\t') {
                start++;
                pos++;
            } 
        } else {
            pos++;
        }
    }

    if (*start && index < MAX_COMMANDS) {
        char* cmd = strdup(start);
        commands->commands[index].command = trim_whitespace(cmd);
        commands->operators[index] = NULL;
        free(cmd);
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

    for (int i = 0; i < command->arg_count; i++) {
        if(args[i] && (strcmp(args[i], REDIRECTORS[0]) == 0 || 
           strcmp(args[i], REDIRECTORS[1]) == 0 || 
           strcmp(args[i], REDIRECTORS[2]) == 0 || 
           strcmp(args[i], REDIRECTORS[3]) == 0)) {
            
            if (args[i + 1]) {
                if (strcmp(args[i], REDIRECTORS[0]) == 0) { // << heredoc
                    command->heredoc_delimiter = strdup(args[i + 1]);
                } else if (strcmp(args[i], REDIRECTORS[1]) == 0) { // >>
                    command->output_redirect = strdup(args[i + 1]);
                    command->append_output = true;
                } else if (strcmp(args[i], REDIRECTORS[2]) == 0) { // <
                    command->input_redirect = strdup(args[i + 1]);
                } else if (strcmp(args[i], REDIRECTORS[3]) == 0) { // >
                    command->output_redirect = strdup(args[i + 1]);
                    command->append_output = false;
                }

                free(args[i]);
                free(args[i + 1]);
                args[i] = NULL;
                args[i + 1] = NULL;
                i++; // Skip next as it's already processed
            }
        }
    }

    // shift args to remove NULL entries
    for (int j = 0; j < command->arg_count; j++) {
        if (args[j] == NULL) {
            for (int k = j; k < command->arg_count - 1; k++) {
                args[k] = args[k + 1];
            }
            command->arg_count--;
            j--; 
        }       
    }
    
    args[command->arg_count] = NULL;
    free(cmd_copy);
}