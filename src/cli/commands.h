#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "commands_def.h"

#define BUFFER_SIZE 1024

/*
struct wifi_conf {
// general
char *ssid;
char *psk;
char *key_mgmt;
int priority;

// eap specific
char *identity;
char *password;
char *proto;
char *pairwise;
char *group;
char *eap;
char *phase2;
};
*/

void process_commands(char **string_input, int *commands);
int run_commands(char **string_input, char *result);
int shift_left(char **string_input, int *commands);
void print_array_c(char **string_input, int size);
void handle_result(char *result);

int handle_delete(char *result);

/*
int handle_list_available(char *result);
int handle_list_configured(char *result);
int handle_conf_manual(struct wifi_conf conf);
int handle_api_init();
*/

