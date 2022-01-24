#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int read_stdin(char *buffer, int buf_len);
char **parse_stdin(char *buffer, int *commands);
void free_data(char *buffer, char *copy, char **string_input, int tokens);
void print_array(char **string_input, int size);
int num_tokens(char *copy);









