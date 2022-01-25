/*
 * Wifi Browser API / wbapi.h
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * 
 * This software is under the terms of the BSD license.
 * See README for more details.
 */
#pragma once

#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
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

/* 
 * api_init - initialize wifi browser api
 * returns: 0 if successful, -1 if fail 
 */
int api_init();

/**************************
 *
 * Configuration File Manipulation
 *
 *************************/

/* 
 * conf_list - list configured networks in configuration file
 * returns: array of ssids as strings
 */
char **conf_list();

/*
 * conf_setDefault - set the default file location for wifi browser api
 * creates the file if the file doesn't exist
 * requires: filepath of configuration file
 * returns: 0 if success, -1 if fail
 */
int conf_setDefault(const char *filepath);

/*
 * conf_setCurrent - set the focused configuration file for wifi browser api
 * creates the file if the file doesn't exist
 * requires: filepath of configuration file
 * returns: 0 if success, -1 if fail
 */
int conf_setCurrent(const char *filepath);

/*
 * conf_connectAuto - adds a new network entry to the focused configuration file
 * and automatically supplies the additional info for
 * wpa_supplicant connection
 * requires: string of ssid, string of passkey for ssid
 * returns: 0 if success, -1 if fail
 */
int conf_connectAuto(char *ssid, char *psk);

/*
 * conf_connectAutoEAP - adds a new eap network entry to the focused configuration file
 * and automatically supplies the additional data needed for
 * wpa_supplicant connection
 * requires: string of ssid, string of username for network,
 *           string of password for user for network
 * returns: 0 if success, -1 if fail
 */
int conf_connectAutoEAP(char *ssid, char *user, char *pwd);

/*
 * conf_connectManual - adds a new network configuration using the information
 * from the provided configuration struct to the focused
 * configuration file
 * requires: wifi_conf struct
 * returns: 0 if success, -1 if fail
 */
int conf_connectManual(struct wifi_conf conf);

/*
 * conf_editNetwork - edits the specified network config (based on ssid) using
 * the information from the provided configuration struct
 * requires: string of ssid to be edited, wifi_conf struct
 * returns: 0 on success, -1 on fail
 */
int conf_editNetwork(char *ssid, struct wifi_conf conf);

/*
 * conf_deleteNetwork - deletes the specified network (by ssid) from the focused
 * configuration file
 * requires: string of ssid to be deleted
 * returns: 0 if success, -1 if fail
 */
int conf_deleteNetwork(char *ssid);

/**************************
 *
 * Wifi Network Information Gathering
 *
 *************************/

/*
 * listAvailable - list available visible networks
 * requires: char * buffer pointer, size of buffer
 * returns: length of scan results, 0 - no networks, -1 - failed
 */
size_t listAvailable(char *buf, size_t len);

/**************************
 *
 * wpa_supplicant Communication
 *
 *************************/

/*
 * wpa_restart - restarts wpa_supplicant()
 * returns: 0 if successful, -1 if fail
 */
int wpa_restart();

/*
 * wpa_running - check if wpa_supplicant is running
 * 1 if wpa_supplicant is running, 0 if not
 */
int wpa_running();

/*
 * wpa_multiInterface - start/restart wpa_supplicant with the multi-interface
 * option enabled
 * requires: string of first interface, string of filepath to first conf file
 *           string of second interface, string of filepath to second conf file
 * returns: 0 if success, -1 if fail
 */
int wpa_multiInterface(char *inf1, char *conf_file1, char *inf2, char *conf_file2);
