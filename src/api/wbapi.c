
/*
 * Wifi Browser API / wbapi.c
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * 
 * This software is under the terms of the BSD license.
 * See README for more details.
 *
 * Central point for processing requests made against Wifi Browser API
 */

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
// returns: 1 if success, 0 if fail
int conf_setDefault(char *conf_file)
{
  return 0;
}

// set the focused configuration file for wifi browser api
// requires: filepath of configuration file
// returns: 1 if success, 0 if fail
int conf_setCurrent(char *conf_file)
{
  // create new file if non-existent
  return 0;
}

// adds a new network entry to the focused configuration file
// and automatically supplies the additional info for
// wpa_supplicant connection
// requires: string of ssid, string of passkey for ssid
// returns: 1 if success, 0 if fail
int conf_configAuto(char *ssid, char *psk)
{
  return 0;
}

// adds a new eap network entry to the focused configuration file
// and automatically supplies the additional data needed for
// wpa_supplicant connection
// requires: string of ssid, string of username for network,
//           string of password for user for network
// returns: 1 if success, 0 if fail
int conf_configAutoEAP(char *ssid, char *user, char *pwd)
{
  return 0;
}

// adds a new network configuration using the information
// from the provided configuration struct to the focused
// configuration file
// requires: wifi_conf struct
// returns: 1 if success, 0 if fail
int conf_configManual(struct wifi_conf conf)
{
  return 0;
}

// edits the specified network config (based on ssid) using
// the information from the provided configuration struct
// requires: string of ssid to be edited, wifi_conf struct
// returns: 1 on success, 0 on fail
int conf_editNetwork(char *ssid, struct wifi_conf conf)
{
  return 0;
}

// deletes the specified network (by ssid) from the focused
// configuration file
// requires: string of ssid to be deleted
// returns: 1 if success, 0 if fail
int conf_deleteNetwork(char *ssid)
{
  return 0;
}



/**************************
 *
 * Wifi Network Information Gathering
 *
 *************************/

// list available visible networks
// returns: array of ssids as strings
char **listAvailable()
{
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
// returns: 1 if successful, 0 if fail
int wpa_restart()
{
  return 0;
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
// returns: 1 if success, 0 if fail
int wpa_multiInterface(char *inf1, char *conf_file1, 
    char *inf2, char *conf_file2)
{
  return 0;
}
