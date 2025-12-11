#ifndef INTERNAL_COMMANDS_H
#define INTERNAL_COMMANDS_H

#include "typedef.h"

extern char *work_dir;

void handle_cd(char** args);
void handle_pwd(char** args);
void handle_echo(char** args);
void handle_history(char** args);
void handle_exit(Commands* commands);

void free_if_needed(void* to_free);
void free_commands(Commands* commands);

#endif // INTERNAL_COMMANDS_H