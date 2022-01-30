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
    
    while(1){

        PRINT_COMMAND_PROMPT;
        buffer = malloc(sizeof(char) * BUFFER_SIZE);
        int data = read_stdin(buffer, BUFFER_SIZE);

        if(!data) {

            PRINT_COMMAND_RESPONSE("No data entered.\n");
            continue;
        }

        char *copy = malloc(sizeof(char) * strlen(buffer));
        bzero(copy, strlen(buffer));
        strcpy(copy, buffer);
        int commands = num_tokens(copy);
        char **string_input = parse_stdin(buffer, &commands); 
        process_commands(string_input, &commands); 
        free_data(buffer, copy, string_input, commands); 
    }
}

/* Return true if number of characters is greater than zero */
int read_stdin(char *buffer, int buffer_len) {

    bzero(buffer, buffer_len);
    if (fgets(buffer, buffer_len, stdin) != NULL) {

        return strlen(buffer) > 1; 
    }

    return 0;
}

/* Count the number of commands from user input */
int num_tokens(char *buffer){

    int count = 0;
    char *token = strtok(buffer, " ");

    while(token != NULL){

        if(!(strlen(token) < 1) && (strcmp(token, "\n") != 0)){

            count++;
        }

        token = strtok(NULL, " ");
    }
    
    return count;
}

/* Parse user input and return an array of strings */
char **parse_stdin(char *buffer, int *commands){

    char **string_input = malloc(sizeof(char *) * *commands);
    bzero(string_input, sizeof(char *) * *commands);
    char *token = strtok(buffer, " "); 
    int idx = 0;

    while(token != NULL){

        if(!(strlen(token) < 1) && (strcmp(token, "\n") != 0)){

            string_input[idx] = malloc(sizeof(token));
            bzero(string_input[idx], strlen(token));
            strcpy(string_input[idx], token); 
            idx++; 
        }

        token = strtok(NULL, " ");
    }

    /* Trim newline character of last token */
    if(string_input[*commands - 1][strlen(string_input[*commands - 1]) - 1] == '\n')
        string_input[*commands - 1][strlen(string_input[*commands - 1]) - 1] = '\0';

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

            if(string_input[i])
                free(string_input[i]);
        }

        free(string_input);
    }
}

/* Print array for testing */
void print_array(char **string_input, int size){

    printf("Print array: \n");
    for(int i = 0; i < size; i++){

        printf("idx: %d  size: %ld  string: %s \n", i, strlen(string_input[i]), string_input[i]);
    }
}


