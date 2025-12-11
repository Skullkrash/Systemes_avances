#ifndef PARSER_H
#define PARSER_H

#include "typedef.h"

void parse_command(Command* command);
void split_line(const char* line, Commands *commands);

#endif