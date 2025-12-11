#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "typedef.h"
#include "internal_commands.h"

int execute_command(Command* command);
void execute_commands(Commands* commands);
int create_child_process(Command* command);

#endif