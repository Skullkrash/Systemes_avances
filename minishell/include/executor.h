#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "typedef.h"
#include "internal_commands.h"

void execute_command(Command* command);
void create_child_process(Command* command);

#endif