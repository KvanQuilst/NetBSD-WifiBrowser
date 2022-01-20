/*
 * Wifi Browser API / wbapi.c
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * 
 * This software is under the terms of the BSD license.
 * See README for more details.
 *
 * Central point for processing requests made against Wifi Browser API
 */

#include "wbapi.h"

/**************************

  Global Variables

**************************/

int currFile;
struct wpa_ctrl *wpa;

// initialize wifi browser api
// returns: 0 if successful, -1 if fail
int api_init()
{
  wpa = wpa_ctrl_open(NULL);
  if (wpa == NULL)
    return -1;

  return 0;
}

/**************************
 *
 * Configuration File Manipulation
 *
 *************************/

// list configured networks in configuration file
// returns: array of ssids as strings
char **conf_list()
{
  return NULL;
}

// set the default file location for wifi browser api
// requires: filepath of configuration file
// returns: 0 if success, -1 if fail
int conf_setDefault(const char *conf_file)
{
  return -1;
}

// set the focused configuration file for wifi browser api
// requires: filepath of configuration file
// returns: 0 if success, -1 if fail
int conf_setCurrent(const char *filepath)
{
/*  int fd;

  fd = open(filepath, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("open");
    fprintf(stderr, "wifi browser api: unable to open %s", filepath);
    return -1;
  }*/
  return -1;
}

// adds a new network entry to the focused configuration file
// and automatically supplies the additional info for
// wpa_supplicant connection
// requires: string of ssid, string of passkey for ssid
// returns: 0 if success, -1 if fail
int conf_configAuto(char *ssid, char *psk)
{
  return -1;
}

// adds a new eap network entry to the focused configuration file
// and automatically supplies the additional data needed for
// wpa_supplicant connection
// requires: string of ssid, string of username for network,
//           string of password for user for network
// returns: 0 if success, -1 if fail
int conf_configAutoEAP(char *ssid, char *user, char *pwd)
{
  return -1;
}

// adds a new network configuration using the information
// from the provided configuration struct to the focused
// configuration file
// requires: wifi_conf struct
// returns: 0 if success, -1 if fail
int conf_configManual(struct wifi_conf conf)
{
  return -1;
}

// edits the specified network config (based on ssid) using
// the information from the provided configuration struct
// requires: string of ssid to be edited, wifi_conf struct
// returns: 0 on success, -1 on fail
int conf_editNetwork(char *ssid, struct wifi_conf conf)
{
  return -1;
}

// deletes the specified network (by ssid) from the focused
// configuration file
// requires: string of ssid to be deleted
// returns: 0 if success, -1 if fail
int conf_deleteNetwork(char *ssid)
{
  return -1;
}

/**************************
 *
 * Wifi Network Information Gathering
 *
 *************************/

// list available visible networks
// returns: array of ssids as strings
char *listAvailable()
{
  char *buf;
  size_t len;

  if (wpa_ctrl_request(wpa, "SCAN", 4, NULL, NULL, NULL) < 0)
    return NULL;

  if (wpa_ctrl_request(wpa, "SCAN_RESULTS", strlen("SCAN_RESULTS"), buf, &len, NULL) < 0) 
    return NULL;
  
  return NULL;
}

// hash a passkey against the associated ssid for 
// use in a configuration file
// DOES NOT AUTOMATICALLY INSERT INTO CONFIG FILE
// requires: string of ssid, string of passkey
// returns: string of hashed passkey
char *hashPsk(char *ssid, char *psk)
{
  return NULL;
}

// hash a password using openssl for use in
// a configuration file
// DOES NOT AUTOMATICALLY INSERT INTO CONFIG FILE
// requires: string of password
// returns: string of hashed password
char *haskPwd(char *pwd)
{
  return NULL;
}

// determines the key management protocol of the
// specified ssid and updates it in the configuration
// struct
// requires: string of ssid, reference to wifi_conf struct of interest
void getKeyMgmt(char *ssid, struct wifi_conf *conf)
{
  return;
}


/**************************
 *
 * wpa_supplicant Communication
 *
 *************************/

// restarts wpa_supplicant()
// returns: 0 if successful, -1 if fail
int wpa_restart()
{
  return -1;
}

// check if wpa_supplicant is running
// 1 if wpa_supplicant is running, 0 if not
int wpa_running()
{
  return 0;
}

// start/restart wpa_supplicant with the multi-interface
// option enabled
// requires: string of first interface, string of filepath to first conf file
//           string of second interface, string of filepath to second conf file
// returns: 0 if success, -1 if fail
int wpa_multiInterface(char *inf1, char *conf_file1, 
    char *inf2, char *conf_file2)
{
  return -1;
}
