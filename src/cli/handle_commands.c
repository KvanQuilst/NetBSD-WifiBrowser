#include "commands.h"
#include "../api/wbapi.h"

void handle_conf_configAutoEAP(){


    
}

void handle_conf_configAuto(){

    char *ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    char *psk = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("ssid: "); fgets(ssid, WIFI_CONF_FIELD, stdin);
    printf("psk: "); fgets(psk, WIFI_CONF_FIELD, stdin);
    if(conf_configAuto(ssid, psk) == -1){

        printf("Error setting config auto\n");
    }

    else{

        printf("Success!\n");
    }
    
    free(ssid);
    free(psk);
}

void handle_conf_setCurrent(){

    char *filepath = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("File path: "); fgets(filepath, WIFI_CONF_FIELD, stdin);
    if(conf_setCurrent(filepath) == -1){

        printf("Error setting filepath\n");
    }

    else{

        printf("Success!\n");
    }

    free(filepath);
}

void handle_list_configured(){

    char *result = malloc(sizeof(char) * BUFFER_SIZE);
    bzero(result, BUFFER_SIZE);
    if(listConfigured(result, BUFFER_SIZE) == 0){

        printf("No networks available.\n");
    }

    else if(strlen(result) > 0){

        printf("%s\n", result); 
    }

    free(result);
}

void handle_list_available(){

    char *result = malloc(sizeof(char) * BUFFER_SIZE);
    bzero(result, BUFFER_SIZE);
    if(listAvailable(result, BUFFER_SIZE) == 0){

        printf("No networks available.\n");
    }

    else if(strlen(result) > 0){

        printf("%s\n", result); 
    }

    free(result);
}

void handle_api_init(){

    api_init();
}







