#include "commands.h"

void process_commands(char **string_input, int *commands){

    while(*commands != 0){

        if(strlen(string_input[0]) == 0){

            shift_left(string_input, commands);
            continue;
        }

        else if(!run_commands(string_input)){

            break;
        }        

        shift_left(string_input, commands);
    }
}

int run_commands(char **string_input){

    if(strcmp(WBCLI, string_input[0]) == 0){

        handle_api_init();
        return 1;
    }

    else if(strcmp(LIST_AVAILABLE, string_input[0]) == 0){

        handle_list_available();
        return 1;
    }

    else if(strcmp(CONF_MANUAL, string_input[0]) == 0){

        handle_conf_manual();
        return 1;
    }

    else if(strcmp(CONF_AUTO, string_input[0]) == 0){

        handle_conf_auto();
        return 1;
    }

    else if(strcmp(LIST_CONFIG, string_input[0]) == 0){

        handle_list_configured();
        return 1;
    }

    else if(strcmp(CLEAR, string_input[0]) == 0){

        system("clear");
        return 1;
    }
    
    else if(strcmp(EXIT, string_input[0]) == 0){

        exit(EXIT_SUCCESS);
    }

    else{

        PRINT_COMMAND_RESPONSE("Invalid command: \n");
        return 0;
    }
} 

/* Shift array to the left to process next command */
int shift_left(char **string_input, int *commands){

    for(int idx = 0; idx < (*commands - 1); idx++){

        bzero(string_input[idx], strlen(string_input[idx]));
        string_input[idx] = realloc(string_input[idx], strlen(string_input[idx + 1]));
        strcpy(string_input[idx], string_input[idx + 1]);
    }

    *commands -= 1;
    return 0; 
}

/* Print array for testing */
void print_array_c(char **string_input, int size){

    printf("Print array: \n");
    for(int i = 0; i < size; i++){

        printf("idx: %d  size: %ld  string: %s \n", i, strlen(string_input[i]), string_input[i]);
    }
}



