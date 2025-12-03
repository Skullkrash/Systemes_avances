#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Découpe une ligne de commande en tokens (arguments).
// Retourne un tableau de chaînes de caractères terminé par NULL.
// L'appelant doit libérer le tableau retourné.
char** parse_command(char* line);

#endif // PARSER_H