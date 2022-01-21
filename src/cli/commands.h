#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "commands_def.h"

void process_commands(char **string_input, int *commands);
void run_commands(char **string_input, int *commands, char *command);
int shift_left(char **string_input, int *commands);
