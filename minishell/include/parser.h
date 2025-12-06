#ifndef PARSER_H
#define PARSER_H

#include "typedef.h"

// Découpe une ligne de commande en tokens (arguments).
// Retourne un tableau de chaînes de caractères terminé par NULL.
// L'appelant doit libérer le tableau retourné.
void parse_command(Command *command);

#endif // PARSER_H