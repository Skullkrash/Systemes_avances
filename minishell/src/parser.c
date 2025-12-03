// parser.c
// fichier contenant le necessaire pour parser les commandes pour le minishell
// BRENNER Quentin, NEAGELY Jeannot
// EICNAM 2025-2026

#include "../include/parser.h"

char** parse_command(char* line)
{
    char** args = (char**)malloc(sizeof(char*) * (strlen(line) + 1));

    char* cmd = strtok(line, " ");
    int i = 0;

    while(cmd != NULL) {
        args[i] = cmd;
        i++;
        cmd = strtok(NULL, " ");
    }
    
    args[i] = NULL; 

    return args;
}