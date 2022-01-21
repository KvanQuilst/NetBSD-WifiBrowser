#include "commands.h"

void process_commands(char **string_input, int *commands){

    char *command; 
    while(*commands != 0){
        
        command = malloc(sizeof(char) * strlen(string_input[0]));
        strncpy(command, string_input[0], strlen(string_input[0])); 
        run_commands(string_input, &commands, command);
        shift_left(string_input, commands);
    }
}

void run_commands(char **string_input, int *commands, char *command){

    if(strcmp(WBCLI, command) == 0){

        
    }

    else if(strcmp(STATUS, command) == 0){


    }

    else if(strcmp(LIST, command) == 0){


    }

    else if(strcmp(CONNECT, command) == 0){

        
    }

    else if(strcmp(NEW_NETWORK, command) == 0){


    }

    else if(strcmp(SET, command) == 0){

        
    }

    else if(strcmp(SELECT, command) == 0){


    }

    else if(strcmp(LIST_CONFIG, command) == 0){

        
    }
    
    else{

        printf("Command not recognized.\n");
    }
} 

/* Shift array to the left to process next command */
int shift_left(char **string_input, int *commands){
    
    for(int idx = 0; idx < (*commands - 1); idx++){

        memset(string_input[idx], '\0', strlen(string_input[idx]));
        string_input[idx] = realloc(string_input[idx], strlen(string_input[idx + 1]));
        memcpy(string_input[idx], string_input[idx + 1], strlen(string_input[idx + 1]));
    }

    *commands -= 1;
    return 0; 
}

