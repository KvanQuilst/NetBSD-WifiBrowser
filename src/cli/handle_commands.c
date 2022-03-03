#include "commands.h"
#include "../api/wbapi.h"

void handle_conf_deleteNetwork(){

    char *ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("ssid: "); fgets(ssid, WIFI_CONF_FIELD, stdin);
    if(conf_deleteNetwork(ssid) < 0){

        printf("Error delete network\n");
    }

    else{

        printf("Success delete network\n");
    }
    
    free(ssid);
}

void handle_conf_enableNetwork(){

    char *ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("ssid: "); fgets(ssid, WIFI_CONF_FIELD, stdin);
    if(conf_enableNetwork(ssid) < 0){

        printf("Error enable network\n");
    }

    else{

        printf("Success enable network\n");
    }
    
    free(ssid);
}

void handle_conf_addEntry(){

    char *ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("ssid: "); fgets(ssid, WIFI_CONF_FIELD, stdin);
    
    char *fields = malloc(sizeof(char) * WIFI_CONF_FIELD); 
    printf("Additional field (y / n): "); fgets(fields, WIFI_CONF_FIELD, stdin);
    while(strcmp(fields, "n") != 0) { 
    
        char *field = malloc(sizeof(char) * WIFI_CONF_FIELD);
        char *value = malloc(sizeof(char) * WIFI_CONF_FIELD);
        printf("field: "); fgets(field, WIFI_CONF_FIELD, stdin);
        printf("value: "); fgets(value, WIFI_CONF_FIELD, stdin);
        if(conf_editNetwork(ssid, field, value) == -1){

            printf("Error edit network\n");   
        }

        else{

            printf("Success edit network\n");
        }

        bzero(fields, WIFI_CONF_FIELD);
        printf("Additional field (y / n): "); fgets(fields, WIFI_CONF_FIELD, stdin);
        free(field);
        free(value);
    }
    
    free(ssid);
    free(fields);
}

void handle_conf_configAutoEAP(){

    char *ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    char *user = malloc(sizeof(char) * WIFI_CONF_FIELD);
    char *pwd = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("ssid: "); fgets(ssid, WIFI_CONF_FIELD, stdin);
    printf("user: "); fgets(user, WIFI_CONF_FIELD, stdin);
    printf("pwd: "); fgets(pwd, WIFI_CONF_FIELD, stdin);

    if(conf_configAutoEAP(ssid, user, pwd) == -1){

        printf("Error config auto EAP\n");
    }
    
    else{

        printf("Success config auto eap\n");
    }

    free(ssid);
    free(user);
    free(pwd);
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

        printf("Success config auto\n");
    }
    
    free(ssid);
    free(psk);
}

void handle_conf_setCurrent(){

    char *filepath = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("File path: "); fgets(filepath, WIFI_CONF_FIELD, stdin);
    if(conf_setCurrent(filepath) < 0){

        printf("Error setting filepath\n");
    }

    else{

        printf("Success conf set Current\n");
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

    else{

        printf("Error: List configured\n");
    }

    free(result);
}

void handle_list_available(){

    char result[BUFFER_SIZE];
    if(listAvailable(result, BUFFER_SIZE) == 0){

        printf("No networks available.\n");
    }

    else if(strlen(result) > 0){

        printf("set fault? \n");
        printf("%s\n", result); 
    }

    else{

        printf("Error: List available\n");
    }
}

void handle_api_init(){

    api_init();
}







