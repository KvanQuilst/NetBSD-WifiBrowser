/*
 * Wifi Browser API / wbapi.h
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * 
 * This software is under the terms of the BSD license.
 * See README for more details.
 */

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "wpa_ctrl.h"

struct wifi_conf {
  // general
  char *ssid;
  char *psk;
  char *key_mgmt;
  int priority;

  // eap specific
  char *user;
  char *password;
  char *proto;
  char *pairwise;
  char *group;
  char *eap;
  char *phase2;
};

int init();

/* Configuration File Manipulation */
char **conf_list();
int conf_setDefault(const char *filepath);
int conf_setCurrent(const char *filepath);
int conf_connectAuto(char *ssid, char *psk);
int conf_connectAutoEAP(char *ssid, char *user, char *pwd);
int conf_connectManual(struct wifi_conf conf);
int conf_editNetwork(char *ssid, struct wifi_conf conf);
int conf_deleteNetwork(char *ssid);

/* Wifi Network Information Gathering */
char **listAvailable();
char *hashPsk(char *ssid, char *psk);
char *hashPwd(char *pwd);
void getKeyMgmt(char *ssid, struct wifi_conf *conf);

/* wpa_supplicant Communication */
int wpa_restart();
int wpa_running();
int wpa_multiInterface(char *inf1, char *conf_file1, char *inf2, char *conf_file2);
