/*
 * Wifi Browser CLI / main
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * 
 * This software is under the terms of the BSD license.
 * See README for more details.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

char *buffer;
int read_stdin(char *buffer, int buf_len);
void user_interface();

int main(int argc, char *argv[]) {
    
    buffer = malloc(BUFFER_SIZE);
    user_interface();
    
    int data = read_stdin(buffer, BUFFER_SIZE);
}


int read_stdin(char *buffer, int buffer_len) {

    bzero(buffer, buffer_len);
    if (fgets(buffer, buffer_len, stdin) != NULL) {

        return strlen(buffer) > 0; //  Return true if num characters > 0 
    }

    return 0;
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

    /*
    * Idea for installation: Check if files are located. Should we write a file to our directory that contains the file paths? 
    * When a user downloads our api from terminal, it should install in their current directory. 
    * Upon loading our api, we will check for a file in our directory called filepaths.txt
    * If the filePaths.txt does not exist, we will create one upon starting our program, and ask the user to enter default file locations 
    * We will copy the files to the directories entered by the user. We will place filePaths.txt in the same directory as our api 
    */

    printf("1. Use Default Connection (Parse data already stored in wpa_supplicant config)\n");
    printf("2. List Available Connections (List available wifi-connections, user selects, parses info to wpa_supplicant config)\n");
    printf("3. Setup New Connection (Manually enter new data to wpa_supplicant config)\n");
    printf("4. Make changes to existing connection (Allow user to change password / other info stored in already existing wpa_supplicant config connection)\n");
    printf("> "); 

    
    
}

