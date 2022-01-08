/*
 * Wifi Browser API / wbapi.h
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * 
 * This software is under the terms of the BSD license.
 * See README for more details.
 */

struct wifi_conf {
  // general
  char *ssid;
  char *psk;
  char *key_mgmt;

  // eap specific
  char *user;
  char *password;
};

char **list_available();
char **list_configured();
int set_default_conf(char *conf_file);
int set_current_conf(char *conf_file);
int connect_auto(char *ssid, char *passkey);
int connect_auto_eap(char *ssid, char *user, char *password);
int connect_manual(struct wifi_conf *conf);
int edit_configured(char *ssid, struct wifi_conf *conf);
int delete_configured(char **ssid);
int multi_interface(char *inf1, char *conf_file1, char *inf2, char *conf_file2);
