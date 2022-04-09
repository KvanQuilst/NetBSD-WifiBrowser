/*
 * Surf API / surf.h
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * 
 * This software is under the terms of the BSD license.
 * See README for more details.
 */
#pragma once

/* 
 * surf_init - initialize the surf api
 * returns: 0 if successful, -1 if fail 
 */
int surf_init();

/**************************
 *
 * Configuration File Manipulation
 *
 *************************/

/*
 * conf_connectAuto - adds a new network entry to the focused configuration file
 * and automatically supplies the additional info for wpa_supplicant connection
 * Set PSK to NULL if no passkey for network
 * requires: string of ssid, string of passkey for ssid
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
 * conf_addEntry - adds a new network configuration entry for the provided
 * ssid. This is intended to be used when manually configuring in conjunction
 * with edit network 
 * NETWORK IS DISABLED WHEN FIRST MADE. MAKE EDITS WITH conf_editNetwork()
 * THEN ENABLE WITH conf_enableNetwork()
 * requires: string of ssid
 * returns: 0 if success, -1 if fail
 */
int conf_addEntry(const char *ssid);

/*
 * conf_editNetwork - edits the specified field of a network config (based on ssid) 
 * to the provided value
 * Automatically disables the network: must re-enable for network configuration
 * to work. 
 * !!! You MUST re-enable the network for it save. Network configuration will be 
 * deleted otherwise. !!!
 * requires: string of ssid name, string of field name, string of value
 * returns: 0 on success, -1 on fail
 */
int conf_editNetwork(const char *ssid, const char *field, const char *value);

/*
 * conf_enableNetwork - Enables a disabled network
 * requires: string of ssid name
 * returns: 0 on success, -1 on fail
 */
int conf_enableNetwork(const char *ssid);

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
