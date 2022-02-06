#include "commands.h"
#include "../api/wbapi.h"

int handle_delete(char *result){

    return 1;
}

int handle_list_configured(char *result){

    if(listConfigured(result, BUFFER_SIZE) == 0){

        printf("No configured networks.\n");
        return 1;
    }

    else if(strlen(result) > 0){

        printf("%s\n", result);
        return 1;
    }

    else{

        return 0;
    }
}

int handle_list_available(char *result){

    if(listAvailable(result, BUFFER_SIZE) == 0){

        printf("No networks available.\n");
        return 1;
    }

    else if(strlen(result) > 0){

        printf("%s\n", result);
        return 1;
    }

    else{

        return 0;
    }
}

int handle_api_init(){
    if(api_init() != -1)
        return 1;

    return 0;
}








