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

typedef struct _wifi_conf wifi_conf;

struct _wifi_conf {
  // general
  const char *ssid;
  const char *psk;
  const char *key_mgmt;
  int priority;

  // eap specific
  const char *identity;
  const char *password;
  const char *proto;
  const char *pairwise;
  const char *group;
  const char *eap;
  const char *phase2;
};

/* 
 * api_init - initialize wifi browser api
 * returns: 0 if successful, -1 if fail 
 */
int api_init();

/*
 * wc_init - initialize a wifi_conf struct
 * returns: initialized wifi_conf struct
 */
wifi_conf wc_init();

/**************************
 *
 * Configuration File Manipulation
 *
 *************************/

/*
 * conf_setDefault - set the default file location for wifi browser api
 * creates the file if the file doesn't exist
 * requires: filepath of configuration file
 * returns: 0 if success, -1 if fail
 */
// NOT AVAILABLE
//int conf_setDefault(const char *filepath);

/*
 * conf_setCurrent - set the focused configuration file for wifi browser api
 * creates the file if the file doesn't exist
 * requires: filepath of configuration file
 * returns: 0 if success, -1 if fail
 */
int conf_setCurrent(const char *filepath);

/*
 * conf_connectAuto - adds a new network entry to the focused configuration file
 * and automatically supplies the additional info for wpa_supplicant connection
 * Set PSK to NULL if no passkey for network
 * requires: string of ssid, length of ssid, string of passkey for ssid, length of psk
 * returns: 0 if success, -1 if fail
 */
int conf_configAuto(const char *ssid, const char *psk);

/*
 * conf_connectAutoEAP - adds a new eap network entry to the focused configuration file
 * and automatically supplies the additional data needed for
 * wpa_supplicant connection
 * requires: string of ssid, string of username for network,
 *           string of password for user for network
 * returns: 0 if success, -1 if fail
 */
int conf_configAutoEAP(const char *ssid, const char *user, const char *pwd);

/*
 * conf_connectManual - adds a new network configuration using the information
 * from the provided configuration struct to the focused
 * configuration file
 * requires: wifi_conf struct
 * returns: 0 if success, -1 if fail
 */
int conf_configManual(wifi_conf conf);

/*
 * conf_editNetwork - edits the specified network config (based on ssid) using
 * the information from the provided configuration struct
 * requires: string of ssid name, string of field name, string of value
 * returns: 0 on success, -1 on fail
 */
int conf_editNetwork(const char *ssid, const char *field, const char *value);

/*
 * conf_deleteNetwork - deletes the specified network (by ssid) from the focused
 * configuration file
 * requires: string of ssid to be deleted
 * returns: 0 on success, -1 on fail
 */
int conf_deleteNetwork(const char *ssid);

/*
 * conf_cleanNetworks - remove all networks from the selected configuration file
 * returns: 0 on success, -1 on fail
 */
int conf_cleanNetworks(void);

/**************************
 *
 * Wifi Network Information Gathering
 *
 *************************/

/* 
 * listConfigured - list configured networks in configuration file
 * requires: buffer pointer, buffer size
 * returns: size of returned buffer
 */
int listConfigured(char *buf, size_t len);

/*
 * listAvailable - list available visible networks
 * requires: buffer pointer, buffer size
 * returns: length of scan results, 0 - no networks, -1 - failed
 */
int listAvailable(char *buf, size_t len);

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
