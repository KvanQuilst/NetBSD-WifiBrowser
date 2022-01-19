#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int read_stdin(char *buffer, int buf_len);
char **parse_stdin(char *buffer, int numCommands);
int num_commands(char *buffer);
void freeData(char *buffer, char **string_input);
void user_interface();













