#include "commands.h"
#include "../api/wbapi.h"

void handle_conf_manual(){

    struct _wifi_conf new_wifi_conf;
    new_wifi_conf.ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    new_wifi_conf.psk = malloc(sizeof(char) * WIFI_CONF_FIELD);
    new_wifi_conf.key_mgmt = malloc(sizeof(char) * WIFI_CONF_FIELD);
    new_wifi_conf.identity = malloc(sizeof(char) * WIFI_CONF_FIELD);
    new_wifi_conf.password = malloc(sizeof(char) * WIFI_CONF_FIELD);
    new_wifi_conf.proto = malloc(sizeof(char) * WIFI_CONF_FIELD);
    new_wifi_conf.pairwise = malloc(sizeof(char) * WIFI_CONF_FIELD);
    new_wifi_conf.group = malloc(sizeof(char) * WIFI_CONF_FIELD);
    new_wifi_conf.eap = malloc(sizeof(char) * WIFI_CONF_FIELD);
    new_wifi_conf.phase2 = malloc(sizeof(char) * WIFI_CONF_FIELD);
    new_wifi_conf.priority = 0;

    printf("ssid: "); fgets(new_wifi_conf.ssid, WIFI_CONF_FIELD, stdin);
    printf("psk: "); fgets(new_wifi_conf.psk, WIFI_CONF_FIELD, stdin);
    printf("key_mgmt: "); fgets(new_wifi_conf.key_mgmt, WIFI_CONF_FIELD, stdin);
    printf("priority: "); scanf("%d", &new_wifi_conf.priority); while((getchar()) != '\n');
    printf("identity: "); fgets(new_wifi_conf.identity, WIFI_CONF_FIELD, stdin);
    printf("password: "); fgets(new_wifi_conf.password, WIFI_CONF_FIELD, stdin);
    printf("proto: "); fgets(new_wifi_conf.proto, WIFI_CONF_FIELD, stdin);
    printf("pairwise: "); fgets(new_wifi_conf.pairwise, WIFI_CONF_FIELD, stdin);
    printf("group: "); fgets(new_wifi_conf.group, WIFI_CONF_FIELD, stdin);
    printf("eap: "); fgets(new_wifi_conf.eap, WIFI_CONF_FIELD, stdin);
    printf("phase2: "); fgets(new_wifi_conf.phase2, WIFI_CONF_FIELD, stdin);
    conf_configManual(new_wifi_conf);
    free(new_wifi_conf.ssid);
    free(new_wifi_conf.psk);
    free(new_wifi_conf.key_mgmt);
    free(new_wifi_conf.identity);
    free(new_wifi_conf.password);
    free(new_wifi_conf.proto);
    free(new_wifi_conf.pairwise);
    free(new_wifi_conf.group);
    free(new_wifi_conf.eap);
    free(new_wifi_conf.phase2);
}

void handle_conf_auto(){

    char *ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    char *psk = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("ssid: "); fgets(ssid, WIFI_CONF_FIELD, stdin);
    printf("psk: "); fgets(psk, WIFI_CONF_FIELD, stdin);
    conf_configAuto(ssid, psk);
    free(ssid);
    free(psk);
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







