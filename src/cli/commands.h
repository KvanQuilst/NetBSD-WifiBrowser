#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "commands_def.h"

#define BUFFER_SIZE 1024

void process_commands(char **string_input, int *commands);
int run_commands(char **string_input, int *commands);
int shift_left(char **string_input, int *commands);
void print_array_c(char **string_input, int size);
void handle_result(char *result);

int handle_delete(char *result);
//int handle_conf_manual(struct wifi_conf conf);
int handle_list_available(char *result);
int handle_list_configured(char *result);
int handle_api_init();


