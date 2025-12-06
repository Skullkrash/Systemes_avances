#ifndef INTERNAL_COMMANDS_H
#define INTERNAL_COMMANDS_H

#include "typedef.h"

extern char *work_dir;

void handle_cd(char** args);
void handle_pwd();
void handle_echo(char** args);
bool is_exit_command(Command command);

#endif // INTERNAL_COMMANDS_H