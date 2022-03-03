#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "commands_def.h"

#define BUFFER_SIZE 1024
#define WIFI_CONF_FIELD 256

void process_commands(char **string_input, int *commands);
int shift_left(char **string_input, int *commands);
int run_commands(char **string_input);
void print_array_c(char **string_input, int size);
void handle_api_init();
void handle_list_available();
void handle_list_configured();
void handle_conf_setCurrent();
void handle_conf_configAuto();
void handle_conf_configAutoEAP();
void handle_conf_addEntry();
void handle_conf_enableNetwork();
void handle_conf_deleteNetwork();





