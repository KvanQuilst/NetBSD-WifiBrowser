/*
 * Wifi Browser CLI / main
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * 
 * This software is under the terms of the BSD license.
 * See README for more details.
 * 
 */

#include "cli.h"
#include "commands.h"

char *buffer;

int main(int argc, char *argv[]) {
    
    buffer = malloc(sizeof(char) * BUFFER_SIZE);
    int data = read_stdin(buffer, BUFFER_SIZE);
    char *copy = malloc(sizeof(char) * strlen(buffer));
    memcpy(copy, buffer, strlen(buffer));
    int commands = num_tokens(buffer);
    char **string_input = parse_stdin(copy, commands);
    print_array(string_input, commands);
    process_commands(string_input, &commands); 
    print_array(string_input, commands);
    free_data(buffer, copy, string_input, commands);
}

/* Return true if number of characters is greater than zero */
int read_stdin(char *buffer, int buffer_len) {

    bzero(buffer, buffer_len);
    if (fgets(buffer, buffer_len, stdin) != NULL) {

        return strlen(buffer) > 0; 
    }

    return 0;
}

/* Count the number of commands from user input */
int num_tokens(char *buffer){

    int count = 0;
    char *token = strtok(buffer, " ");

    while(token != NULL){

        token = strtok(NULL, " ");
        count++;
    }
    
    return count;
}

/* Parse user input and return an array of strings */
char **parse_stdin(char *buffer, int commands){

    char **string_input = malloc(sizeof(char *) * commands);
    char *token = strtok(buffer, " ");
    int idx = 0;

    while(token != NULL){

        string_input[idx] = malloc(sizeof(char) * strlen(token));
        memcpy(string_input[idx], token, strlen(token));
        token = strtok(NULL, " "); idx++;
    }
    
    /* Trim newline character of last token */
    string_input[commands - 1][strlen(string_input[commands - 1]) - 1] = '\0';
    return string_input;
}

/* Free all memory */
void free_data(char *buffer, char *copy, char **string_input, int tokens){

    if(buffer)
        free(buffer);

    if(copy)
        free(copy);

    if(string_input){

        for(int i = 0; i < tokens; i++){

            free(string_input[i]);
        }

        free(string_input);
    }
}

void print_array(char **string_input, int size){

    printf("Print array: \n");
    for(int i = 0; i < size; i++){

        printf("idx: %d    s: %s \n", i, string_input[i]);
    }
}

/*
* 1. Use Default Connection (Parse data already stored in wpa_supplicant config)
* 2. List Available Connections (List available wifi-connections, user selects, parses info to wpa_supplicant config)
* 3. Setup New Connection (Manually enter new data to wpa_supplicant config)
* 4. Make changes to existing connection (Allow user to change password / other info stored in already existing wpa_supplicant config connection) 
* (We can make a man page and some commands later. Only using a number system for simplicity)
*/

void user_interface() { 

    printf("NetBSD Wi-Fi Connection Setup... \n");
    printf("> "); 

    
    
}

