#include "commands.h"

/* Manually adds new entry to configuration file */
void handle_conf_addEntry(){

    char *ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("ssid: "); fgets(ssid, WIFI_CONF_FIELD, stdin);
    
    char *additionalField = malloc(sizeof(char) * WIFI_CONF_FIELD); printf("Additional field (y / n): "); 
    fgets(additionalField, sizeof(char) * WIFI_CONF_FIELD, stdin); 
    while(strcmp(additionalField, "y\n") == 0){

        char *field = malloc(sizeof(char) * WIFI_CONF_FIELD);
        char *value = malloc(sizeof(char) * WIFI_CONF_FIELD);
        printf("field: "); fgets(field, WIFI_CONF_FIELD, stdin);
        printf("value: "); fgets(value, WIFI_CONF_FIELD, stdin);
        if(conf_editNetwork(ssid, field, value) < 0){

            printf("Error edit network\n");   
        }

        else{

            printf("Success edit network\n");
        }
        
        bzero(additionalField, sizeof(char) * WIFI_CONF_FIELD); 
        printf("Additional field (y / n): ");
        fgets(additionalField, sizeof(char) * WIFI_CONF_FIELD, stdin);
        free(field);
        free(value);
    }

    if(conf_enableNetwork(ssid) < 0){

        printf("Error enable network\n");
    }

    else{

        printf("Success enable network\n");
    }

    free(ssid);   
}

/* Enables the specified network in the configuration file */
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

/* Deletes the specified network from the configuration file */
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

/* Adds a new network EAP to the configuration file; automatically configures network */
void handle_conf_configAutoEAP(){

    char *ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    char *user = malloc(sizeof(char) * WIFI_CONF_FIELD);
    char *pwd = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("ssid: "); fgets(ssid, WIFI_CONF_FIELD, stdin);
    printf("user: "); fgets(user, WIFI_CONF_FIELD, stdin);
    printf("pwd: "); fgets(pwd, WIFI_CONF_FIELD, stdin);

    if(conf_configAutoEAP(ssid, user, pwd) < 0){

        printf("Error config auto EAP\n");
    }
    
    else{

        printf("Success config auto eap\n");
    }

    free(ssid);
    free(user);
    free(pwd);
}

/* Adds a new network to the configuration file; automatically configures network */
void handle_conf_configAuto(){

    char *ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    char *psk = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("ssid: "); fgets(ssid, WIFI_CONF_FIELD, stdin);
    printf("psk: "); fgets(psk, WIFI_CONF_FIELD, stdin);
    if(conf_configAuto(ssid, psk) < 0){

        printf("Error setting config auto\n");
    }

    else{

        printf("Success config auto\n");
    }
    
    free(ssid);
    free(psk);
}

/* Sets the default configuration file location */
void handle_conf_setCurrent(){

    char *filepath = malloc(sizeof(char) * WIFI_CONF_FIELD); 
    bzero(filepath, WIFI_CONF_FIELD);
    
    printf("File path: "); fgets(filepath, WIFI_CONF_FIELD, stdin);
    if(conf_setCurrent(filepath) < 0){

        printf("Error config setCurrent \n");
    }

    else{

        printf("Success config setCurrent \n");
    }

    free(filepath);
}

/* Lists all configured networks in the configuration file */
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

/* Lists all available networks */
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

/* Initializes wpa_supplicant interface */
void handle_api_init(){

    printf("Connecting...\n");
    if(api_init() == 0){

        printf("Success!!\n");
        printf("Type command 'help' to view commands\n");
    }
    
    else{

        printf("Error: Cannot connect to wpa_supplicant\n");
        exit(EXIT_FAILURE);
    }
}

/* Display available commands with program */
void handle_manual(){

    printf("Keywords used with CLI: \n");
    printf("list: List all available networks: \n");
    printf("\t1. '-a' Available Networks\n");
    printf("\t2. '-c' Configured Networks\n");
    printf("conf: Used to edit configuration file: \n");
    printf("\t1. '-sc' Set new configuration path\n");
    printf("\t2. 'ca' Auto configuration of network to configuration file\n");
    printf("\t3. 'eap' Auto configuration of new eap network to configuration file\n");
    printf("\t4. 'ae' Add new entry manual configuration to configuration file\n");
    printf("\t5. '-en' Enable specified network in configuration file\n");
    printf("\t6. 'dn' Delete specified network in configuration file\n");
    printf("clear: Clear screen\n");
    printf("exit: Exit command interface\n");
}







