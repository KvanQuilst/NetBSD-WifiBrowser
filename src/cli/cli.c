/*
 * Wifi Browser CLI / main
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * 
 * This software is under the terms of the BSD license.
 * See README for more details.
 * 
 */

#include "cli.h"

char *buffer;

int main(int argc, char *argv[]) {
    
    buffer = malloc(sizeof(char) * BUFFER_SIZE);
    int data = read_stdin(buffer, BUFFER_SIZE);
    char *copy = malloc(sizeof(char) * strlen(buffer));
    memcpy(copy, buffer, strlen(buffer));
    int commands = num_commands(buffer);
    printf("%d\n", commands);



    if(buffer){

        free(buffer);
    }   
}


int read_stdin(char *buffer, int buffer_len) {

    bzero(buffer, buffer_len);
    if (fgets(buffer, buffer_len, stdin) != NULL) {

        return strlen(buffer) > 0; //Return true if num characters > 0 
    }

    return 0;
}

//token the string, count the number of tokens 
int num_commands(char *buffer){

    int count = 0;
    char *token = strtok(buffer, " ");

    while(token != NULL){

        printf("%s\n", token);
        token = strtok(NULL, s);
    }
    
    return count;
}

//Will fix using strtok
char **parse_stdin(char *buffer, int numCommands){

    char **string_input = malloc(sizeof(char *) * numCommands);
    bzero(string_input, sizeof(char *) * numCommands);
    
    int stringIndex = 0; int commandLength = 0; 
    char save[BUFFER_SIZE]; bzero(save, BUFFER_SIZE);
    for(int idx = 0; buffer[idx] != '\0'; idx++){

        if(buffer[idx] == ' ' && buffer[idx + 1] != ' '){

            string_input[stringIndex] = malloc(sizeof(char) * commandLength); 
            bzero(string_input[stringIndex], strlen(string_input[stringIndex]));
            strncpy(string_input[stringIndex], save, strlen(save));
            stringIndex++; commandLength = 0; bzero(save, strlen(save));
        }

        else if(buffer[idx] != ' '){

            save[commandLength] = buffer[idx];
            commandLength++;
        }
    }
    
    return string_input;
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

