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

/* Network configuration */
static FILE *curr_conf;

/* wpa_supplicant related */
static struct wpa_ctrl *wpa;
#define CTRL_IFACE_DIR "/var/run/wpa_supplicant/"
static const char *ctrl_iface_dir = CTRL_IFACE_DIR;
static char *ifname = NULL;


/**************************
  Static Prototypes
**************************/
static FILE *conf_createFile(const char *filepath);
static int wpaReq(const char *cmd, size_t cmd_len, char *repl, size_t repl_len);
static char *hashPsk(char *ssid, char *psk);
static char *hashPwd(char *pwd); 
static void getKeyMgmt(char *ssid, struct wifi_conf *conf);

int api_init()
{
  /* Open default interface directory; look for interface */
  char *iface_dir;
  struct dirent *dent;
  DIR *dir = opendir(ctrl_iface_dir);
  if (dir == NULL) {
    fprintf(stderr, "wbapi: Default interface location does not exist!\n");
    return -1;
  }

  while ((dent = readdir(dir))) {
    if (strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0) {
      ifname = strdup(dent->d_name);
      printf("Current interface: %s\n", ifname ? ifname : "n/a");
    }
  }

  iface_dir = strdup(ctrl_iface_dir);
  strcat(iface_dir, ifname);

  /* Attempt to connect wpa_supplicant instance */
  wpa = wpa_ctrl_open(iface_dir);
  if (wpa == NULL) {
    fprintf(stderr, "wbapi: Unable to connect to wpa_supplicant on this interface!\n");
    return -1;
  }

  /* Grab wbapi config */

  return 0;
}

static FILE *conf_createFile(const char *filepath)
{
  FILE *fp;
  time_t t = time(NULL);

  if (fp = fopen(filepath, "r")) {
    fprintf(stderr, "wbapi: Config file already exists - cannot initialize!\n");
    return NULL;
  }

  if ((fp = fopen(filepath, "w+x")) == NULL) {
    fprintf(stderr, "wbapi: Error creating config file\n");
    return NULL;
  }

  fprintf(fp, "# wpa_supplicant configuration file - auto-created by wbapi at %s\n", ctime(&t));
  
  return fp;
}

int conf_setDefault(const char *conf_file)
{
  return -1;
}

int conf_setCurrent(const char *filepath)
{
  return -1;
}

static int wpaReq(const char *cmd, size_t cmd_len, 
    char *repl, size_t repl_len)
{
  int retval;
  size_t l = repl_len-1;

  retval = wpa_ctrl_request(wpa, cmd, cmd_len, repl, &l, NULL);
  if (retval == -2) {
    fprintf(stderr, "Connection timed out; command dropped: %s\n", cmd);
    return -1;
  } else if (retval < 0) {
    fprintf(stderr, "Command failed: %s\n", cmd);
    return -1;
  } else {
    repl[l] = 0;
    return l;
  }
}

static int removeNetworkId(int netId)
{
  char cmd[128];
  char repl[1];
  sprintf(cmd, "REMOVE_NETWORK %d", netId);
  return wpaReq(cmd, sizeof(cmd)-1, repl, 1) < 0 ? -1 : 0;
}

int conf_configAuto(char *ssid, size_t ssid_len, char *psk, size_t psk_len)
{
  char repl[128];
  char *cmd[128];
  int netId;

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  /* check ssid validity */
  
  if (wpaReq("ADD_NETWORK", 11, repl, 128) < 0) return -1;
  if (!strncmp(repl, "FAIL", 4)) {
    fprintf(stderr, "Network configuration creation failed!\n");
    return -1;
  }

  netId = atoi(repl);
  sprintf(cmd, "SET_NETWORK %d ssid %s", netId, ssid);
  if (wpaReq(cmd, strlen(cmd)-1, repl, 128) < 0) {
    removeNetworkId(netId);
    return -1;
  }
  
  sprintf(cmd, "SET_NETWORK %d psk %s", netId, psk);
  if (wpaReq(cmd, strlen(cmd)-1, repl, 128) < 0) {
    removeNetworkId(netId);
    return -1;
  }

  sprintf(cmd, "ENABLE_NETWORK %d", netId);
  if (wpaReq(cmd, strlen(cmd)-1, repl, 128) < 0) {
    removeNetworkId(netId);
    return -1;
  }

  if (wpaReq("SAVE_CONFIG", 11, repl, 128) < 0) {
    removeNetworkId(netId);
    return -1;
  }

  if (wpaReq("RECONFIGURE", 11, repl, 128) < 0) {
    removeNetworkId(netId);
    return -1;
  }

  return 0;
}

int conf_configAutoEAP(char *ssid, char *user, char *pwd)
{
  return -1;
}

int conf_configManual(struct wifi_conf conf)
{
  return -1;
}

int conf_editNetwork(char *ssid, struct wifi_conf conf)
{
  return -1;
}

int conf_deleteNetwork(char *ssid)
{
  return -1;
}

size_t listConfigured(char *buf, size_t len)
{
  size_t l;

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  return wpaReq("LIST_NETWORKS", 13, buf, len);
}

size_t listAvailable(char *buf, size_t len)
{
  int retval;
  size_t l;

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  if (wpaReq("SCAN", 4, buf, len) < 0) return -1;
  return wpaReq("SCAN_RESULTS", 12, buf, len);
}

// hash a passkey against the associated ssid for 
// use in a configuration file
// DOES NOT AUTOMATICALLY INSERT INTO CONFIG FILE
// requires: string of ssid, string of passkey
// returns: string of hashed passkey
static char *hashPsk(char *ssid, char *psk)
{
  return NULL;
}

// hash a password using openssl for use in
// a configuration file
// DOES NOT AUTOMATICALLY INSERT INTO CONFIG FILE
// requires: string of password
// returns: string of hashed password
static char *hashPwd(char *pwd)
{
  return NULL;
}

// determines the key management protocol of the
// specified ssid and updates it in the configuration
// struct
// requires: string of ssid, reference to wifi_conf struct of interest
static void getKeyMgmt(char *ssid, struct wifi_conf *conf)
{
  return;
}


int wpa_restart()
{
  return -1;
}

int wpa_running()
{
  return 0;
}

int wpa_multiInterface(char *inf1, char *conf_file1, 
    char *inf2, char *conf_file2)
{
  return -1;
}
