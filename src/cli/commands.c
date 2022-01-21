#include "commands.h"

void process_commands(char **string_input, int *commands){

    char *command; 
    while(*commands != 0){
        
        command = malloc(sizeof(char) * strlen(string_input[0]));
        strncpy(command, string_input[0], strlen(string_input[0]));
        shift_left(string_input, commands); 
        run_commands(command);
    }
}


void run_commands(char *string_input_idx){

    if(strcmp(WBCLI, string_input_idx) == 0){

        printf("Wbcli command\n");
    }

    else if(strcmp(LIST, string_input_idx) == 0){

        printf("List command\n");        
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

