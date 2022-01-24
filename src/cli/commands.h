#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "commands_def.h"

void process_commands(char **string_input, int *commands);
int run_commands(char **string_input, int *commands);
int shift_left(char **string_input, int *commands);
void print_array_c(char **string_input, int size);
