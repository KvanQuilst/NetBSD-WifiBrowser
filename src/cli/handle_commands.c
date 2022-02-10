#include "commands.h"
#include "../api/wbapi.h"

int handle_conf_manual(){

    struct wifi_conf new_wifi_conf;
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
    int result = conf_configManual(new_wifi_conf);
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
    
    if(result == -1){

        printf("Error parsing configuration file.\n");
        return 0;
    }

    else{

        return 1;
    }
}

int handle_conf_auto(){

    char *ssid = malloc(sizeof(char) * WIFI_CONF_FIELD);
    char *psk = malloc(sizeof(char) * WIFI_CONF_FIELD);
    printf("ssid: "); fgets(ssid, WIFI_CONF_FIELD, stdin);
    printf("psk: "); fgets(psk, WIFI_CONF_FIELD, stdin);
    int result = conf_configAuto(ssid, psk);
    free(ssid);
    free(psk);

    if(result == -1){

        printf("Error parsing auto configuration.\n");
        return 0;
    }

    else{

        return 1;
    }
}

int handle_list_configured(char *result){

    bzero(result, BUFFER_SIZE);
    if(listConfigured(result, BUFFER_SIZE) == 0){

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

int handle_list_available(char *result){

    bzero(result, BUFFER_SIZE);
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







