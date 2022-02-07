#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "commands_def.h"

#define BUFFER_SIZE 1024
#define WIFI_CONF_FIELD 256

void process_commands(char **string_input, int *commands);
int run_commands(char **string_input, char *result);
int shift_left(char **string_input, int *commands);
void print_array_c(char **string_input, int size);
int handle_list_available(char *result);
int handle_list_configured(char *result);
int handle_conf_manual();
int handle_conf_auto();
int handle_api_init();



