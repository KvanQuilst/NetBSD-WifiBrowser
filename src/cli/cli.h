#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int read_stdin(char *buffer, int buf_len);
char **parse_stdin(char *buffer, int numCommands);
void free_data(char *buffer, char *copy, char **string_input, int tokens);
int num_tokens(char *buffer);
void print_array(char **string_input, int size);
void user_interface();













