// parser.c
// fichier contenant le necessaire pour parser les commandes pour le minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/parser.h"

void parse_command(Command *command)
{
    char** args = (char**)malloc(sizeof(char*) * (strlen(command->command) + 1));
    char* line = strcpy(malloc(strlen(command->command) + 1), command->command);

    char* cmd = strtok(line, " ");
    int i = 0;

    while(cmd != NULL) {
        args[i] = cmd;
        i++;
        cmd = strtok(NULL, " ");
    }
    
    args[i] = NULL; 

    command->args = args;
}