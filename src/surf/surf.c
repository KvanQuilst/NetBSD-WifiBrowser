/*
 * Surf API / surf.c
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * * This software is under the terms of the BSD license.  * See README for more details.
 *
 * Central point for processing requests made against the surf API
 */

#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <openssl/evp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "wpa_ctrl.h"

#include "surf.h"

/**************************
  Global Variables
**************************/

/* wpa_supplicant related */
static struct wpa_ctrl *wpa;
#define CTRL_IFACE_DIR "/var/run/wpa_supplicant/"
static const char *ctrl_iface_dir = CTRL_IFACE_DIR;
static char *ifname = NULL;

#define MAX_SSID_LEN 32

/**************************
  Static Prototypes
**************************/
static FILE *conf_create(const char *filepath);
static char *hashPsk(const unsigned char *ssid, int slen, const unsigned char *psk, 
    int plen);

/**************************
  Helper Functions
**************************/
static void errMsg(char *format, ...) {
  va_list vl;
#ifdef DEBUG
  fprintf(stderr, "surf: ");
  va_start(vl, format);
  vfprintf(stderr, format, vl);
  va_end(vl);
  fprintf(stderr, "\n");
#endif
}

static int wpaReq(const char *cmd, size_t cmd_len, 
    char *repl, size_t repl_len)
{
  int retval;
  size_t l = repl_len-1;

  retval = wpa_ctrl_request(wpa, cmd, cmd_len, repl, &l, NULL);
  if (retval == -2) {
    errMsg("connection timed out; command dropped %s", cmd);
    return -1;
  } else if (retval < 0) {
    errMsg("command failed: %s", cmd);
    return -1;
  } else {
    repl[l] = 0;
    return l;
  }
}

static int reconfigure()
{
  char repl[128] = {0};

  if (!wpa) {
    errMsg("not connected to wpa_supplicant...");
    return -1;
  }
  
  if (wpaReq("RECONFIGURE", 11, repl, 128) < 0) {
    return -1;
  }
  if (strncmp("OK", repl, 2)) {
    errMsg("error in re-reading configuration file!");
    return -1;
  }
  return 0;
}

static int save()
{
  char repl[128] = {0};

  if (wpaReq("SAVE_CONFIG", 11, repl, 128) < 0) {
    return -1;
  }
  if (strncmp("OK", repl, 2)) {
    errMsg("error in saving configuration!");
    return -1;
  }
  return reconfigure();
}

static int getNetworkID(const char *ssid)
{
  int id = 0;
  char cmd[64] = {0};
  char repl[128] = {0};

  while(strncmp(repl, "FAIL", 5)) {
    snprintf(cmd, sizeof(cmd), "GET_NETWORK %d ssid", id);
    if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0) {
      errMsg("error in obtaining network id");
      return -1;
    }
    sscanf(repl, "\"%[^\"]\"", repl);
    if (!strncmp(ssid, repl, MAX_SSID_LEN)) {
      return id;
    }
    id++;
  }

  errMsg("network ssid does not exist!");
  return -1;
}

/************************
  API Functions
************************/

int surf_init()
{
  /* Open default interface directory; look for interface */
  char *iface_dir;
  struct dirent *dent;
  DIR *dir = opendir(ctrl_iface_dir);

  if (dir == NULL) {
    errMsg("default interface location does not exist!");
    return -1;
  }

  while ((dent = readdir(dir))) {
    if (strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0) {
      ifname = strdup(dent->d_name);
      errMsg("current interface: %s", ifname ? ifname : "n/a");
    }
  }

  if (ifname == NULL) {
    errMsg("default interface location does not exist!");
    return -1;
  }

  iface_dir = strdup(ctrl_iface_dir);
  strcat(iface_dir, ifname);

  /* Attempt to connect wpa_supplicant instance */
  wpa = wpa_ctrl_open(iface_dir);
  if (wpa == NULL) {
    fprintf(stderr, "surf: unable to connect to wpa_supplicant on this interface!... "
        "Is wpa_supplicant running?\n");
    return -1;
  }

  return 0;
}

static FILE *conf_create(const char *filepath)
{
  FILE *fp;
  time_t t = time(NULL);

  if ((fp = fopen(filepath, "r")) != NULL) {
    errMsg("config file already exists - cannot initialize!");
    return NULL;
  }

  if ((fp = fopen(filepath, "w+x")) == NULL) {
    errMsg("error creating config file");
    return NULL;
  }

  fprintf(fp, "# wpa_supplicant configuration file - auto-created by surf at %s", ctime(&t));
  fprintf(fp, "ctrl_interface=DIR=%s GROUP=wheel\n", ctrl_iface_dir);
  fprintf(fp, "update_config=1\n");

  fflush(fp);

  return fp;
}

int conf_configAuto(const char *ssid, const char *psk)
{
  char repl[128] = {0};
  char cmd[128] = {0};
  char *hash;
  int id, plen, slen;

  if (!wpa) {
    errMsg("not connected to wpa_supplicant...");
    return -1;
  }

  /* check ssid validity */
  if (ssid == NULL) {
    errMsg("no ssid provided");
    return -1;
  }

  slen = strnlen(ssid, MAX_SSID_LEN+1);
  if (slen == 0 || slen > MAX_SSID_LEN) {
    errMsg("provided ssid is invalid. ssid length: 1-32 characters");
    return -1;
  }

  if (wpaReq("ADD_NETWORK", 11, repl, sizeof(repl)) < 0)
    return -1;
  if (!strncmp("FAIL", repl, 4)) {
    errMsg("failed to add new network");
    return -1;
  }
  id = atoi(repl);

  snprintf(cmd, sizeof(cmd), "SET_NETWORK %d ssid \"%s\"", id, ssid);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    errMsg("failed to set ssid %s", ssid);
    return -1;
  }

  if (psk == NULL)
    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d key_mgmt NONE", id);
  else {
    plen = strnlen(psk, 64);
    if (plen < 8 || plen > 63) {
      errMsg("provided passkey is invalid. passkey length: 8-63 characters");
      return -1;
    }
    hash = hashPsk(ssid, slen, psk, plen);

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d key_mgmt NONE", id);
    if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
      return -1;
    if (strncmp("OK", repl, 2)) {
      errMsg("failed to set key_mgmt");
      return -1;
    }

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d psk %s", id, hash);
  }

  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    errMsg("failed to set field");
    return -1;
  }
  
  return conf_enableNetwork(ssid);
}

int conf_configAutoEAP(const char *ssid, const char *user, const char *pwd)
{
  char repl[128] = {0};
  char cmd[128] = {0};
  int id, slen;

  if (!wpa) {
    errMsg("not connected to wpa_supplicant...");
    return -1;
  }

  /* check ssid validity */
  if (ssid == NULL) {
    errMsg("no ssid provided");
    return -1;
  }

  if (user == NULL) {
    errMsg("no username provided");
    return -1;
  }

  if (pwd == NULL) {
    errMsg("no password provided");
    return -1;
  }

  slen = strnlen(ssid, MAX_SSID_LEN+1);
  if (slen == 0 || slen > MAX_SSID_LEN) {
    errMsg("provided ssid is invalid. ssid length: 1-32 characters");
    return -1;
  }

  if (wpaReq("ADD_NETWORK", 11, repl, sizeof(repl)) < 0)
    return -1;
  if (!strncmp("FAIL", repl, 4)) {
    errMsg("failed to add new network entry");
    return -1;
  }
  id = atoi(repl);

  snprintf(cmd, sizeof(cmd), "SET_NETWORK %d ssid \"%s\"", id, ssid);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    errMsg("failed to add ssid field: %s", ssid);
    return -1;
  }

  snprintf(cmd, sizeof(cmd), "SET_NETWORK %d key_mgmt WPA-EAP", id);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    errMsg("failed to set key management");
    return -1;
  }

  snprintf(cmd, sizeof(cmd), "SET_NETWORK %d identity \"%s\"", id, user);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    errMsg("failed to set username");
    return -1;
  }

  snprintf(cmd, sizeof(cmd), "SET_NETWORK %d password \"%s\"", id, pwd);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    errMsg("failed to set password");
    return -1;
  }

  return conf_enableNetwork(ssid);
}

int conf_addEntry(const char *ssid)
{
  char repl[128] = {0};
  char cmd[128] = {0};
  int id, slen;

  if (!wpa) {
    errMsg("not connected to wpa_supplicant...");
    return -1;
  }

  /* check ssid validity */
  if (ssid == NULL) {
    errMsg("no ssid provided");
    return -1;
  }

  slen = strnlen(ssid, MAX_SSID_LEN+1);
  if (slen == 0 || slen > MAX_SSID_LEN) {
    errMsg("provided ssid is invalid. ssid length: 1-32 characters");
    return -1;
  }

  if (wpaReq("ADD_NETWORK", 11, repl, sizeof(repl)) < 0)
    return -1;
  if (!strncmp("FAIL", repl, 4)) {
    errMsg("error in adding new network");
    return -1;
  }
  id = atoi(repl);

  snprintf(cmd, sizeof(cmd), "SET_NETWORK %d ssid \"%s\"", id, ssid);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    errMsg("error in setting network ssid: %s", ssid);
    return -1;
  }

  snprintf(cmd, sizeof(cmd), "SET_NETWORK %d key_mgmt NONE", id);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    errMsg("error in setting network key_mgmt");
    return -1;
  }

  return save();
}

int conf_editNetwork(const char *ssid, const char *field, const char *value)
{
  char repl[128] = {0};
  char cmd[128] = {0};
  char *hash;
  int id, plen, slen;

  if (!wpa) {
    errMsg("not connected to wpa_supplicant...");
    return -1;
  }

  /* verify ssid */
  if (ssid == NULL) {
    errMsg("no ssid provided\n");
    return -1;
  }

  slen = strnlen(ssid, MAX_SSID_LEN+1);
  if (slen == 0 || slen > MAX_SSID_LEN) {
    errMsg("provided ssid is invalid. ssid length: 1-32 characters");
    return -1;
  }

  id = getNetworkID(ssid);
  if (id < 0)
    return -1;

  snprintf(cmd, sizeof(cmd), "DISABLE_NETWORK %d", id);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    errMsg("error in disabling network %s", ssid);
    return -1;
  }

  if (!strncmp(field, "psk", 4)) {
    plen = strnlen(value, 64);
    if (plen < 8 || plen > 63) {
      errMsg("provided passkey is invalid. passkey length: 8-63 characters");
      return -1;
    }
    hash = hashPsk(ssid, slen, value, plen);
    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d %s %s", id, field, hash);
  } else if (!strncmp(field, "ssid", 5) || !strncmp(field, "identity", 9) ||
      !strncmp(field, "password", 9))
    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d %s \"%s\"", id, field, value);
  else
    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d %s %s", id, field, value);

  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;

  if (strncmp("OK", repl, 2)) {
    errMsg("error in setting network field: %s", field);
    return -1;
  }
  return 0;
}

int conf_enableNetwork(const char *ssid)
{
  char repl[128] = {0};
  char cmd[64] = {0};
  int id, slen;

  if (!wpa) {
    errMsg("not connected to wpa_supplicant...");
    return -1;
  }

  /* verify ssid */
  if (ssid == NULL) {
    errMsg("no ssid provided");
    return -1;
  }

  slen = strnlen(ssid, MAX_SSID_LEN+1);
  if (slen == 0 || slen > MAX_SSID_LEN) {
    errMsg("provided ssid is invalid. ssid length: 1-32 characters");
    return -1;
  }

  id = getNetworkID(ssid);
  if (id < 0)
    return -1;

  snprintf(cmd, sizeof(cmd), "ENABLE_NETWORK %d", id);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    errMsg("error in enabling network %s", ssid);
    return -1;
  }
  return save();
}

int conf_deleteNetwork(const char *ssid)
{
  char repl[128] = {0};
  char cmd[64] = {0};
  int id, slen;

  /* verify ssid */
  if (ssid == NULL) {
    errMsg("no ssid provided");
    return -1;
  }

  slen = strnlen(ssid, MAX_SSID_LEN+1);
  if (slen == 0 || slen > MAX_SSID_LEN) {
    errMsg("provided ssid is invalid. ssid length: 1-32 characters");
    return -1;
  }

  id = getNetworkID(ssid);
  if (id < 0)
    return -1;

  sprintf(cmd, "REMOVE_NETWORK %d", id);
  if (wpaReq(cmd, sizeof(cmd), repl, 128) < 0)
    return -1;

  if (strncmp("OK", repl, 2)) {
    errMsg("error in removing network: %s", ssid);
    return -1;
  }

  return save();
}

int conf_cleanNetworks(void)
{
  char repl[128] = {0};
  char cmd[64] = {0};
  int i = 0;

  while (strncmp(repl, "FAIL", 4)) {
    snprintf(cmd, sizeof(cmd), "REMOVE_NETWORK %d", i);
    if(wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
      return -1;
    i++;
  }
  return save();
}

int listConfigured(char *buf, size_t len)
{
  char list[4096] = {0};
  char ssid[32] = {0};
  char curr[32] = {0};
  int pos = 0;
  int l = 0;

  if (!wpa) {
    errMsg("not connected to wpa_supplicant...");
    return -1;
  }

  if (wpaReq("LIST_NETWORKS", 13, list, sizeof(list)) < 0)
    return -1;

  sscanf(list, "%*[^\n]%[\001-\255]", list);
  if (list[1] == 0)
    snprintf(buf, len, "No configured networks!\n");
  else {
    sscanf(list, "\n%[\001-\255]", list);
    while (l != -1) {
      l = sscanf(list, "%*s%s%*s%[^\n]%[\001-\255]", ssid, curr, list);
      //l = strnlen(list, 10);
      //printf("%d\n", l);
      if (l != -1) {
        snprintf(&buf[pos], len - pos, "%-32s %s\n", ssid, curr);
        pos += 34 + strnlen(curr, sizeof(curr));
      }
    }
  }

  return 0;
}

int listAvailable(char *buf, size_t len)
{
  char list[4096] = {0};
  char ssid[32] = {0};
  char flags[128] = {0};
  char signal[32] = {0};
  int pos = 0;
  int sig;

  if (!wpa) {
    errMsg("not connected to wpa_supplicant...");
    return -1;
  }

  if (wpaReq("SCAN", 4, list, sizeof(list)) < 0) 
    return -1;
  if (wpaReq("SCAN_RESULTS", 12, list, sizeof(list)) < 0) 
    return -1;

  sscanf(list, "%*[^\n]%[\001-\255]", list); 

  if (list[1] == 0)
    snprintf(buf, len, "No networks available!\n");
  else {
    //snprintf(buf, len, "%-32s %-6s %5s\n", "SSID", "SIGNAL", "FLAGS");
    //pos += 46;
    sscanf(list, "\n%[\001-\255]", list);
    while (list[1] != 0) {
      sscanf(list, "\n%*s%*s%s%s\t%[^\n]%[\001-\255]", signal, flags, ssid, list);
      sig = atoi(signal);
      snprintf(&buf[pos], len - pos, "%-32s %-6d %s\n", ssid, sig, flags);
      pos += 41 + strnlen(flags, sizeof(flags));
    }
  }
  return 0;
}

// hash a passkey against the associated ssid for 
// use in a configuration file
// requires: string of ssid, length of ssid, string of passkey, length of passkey
// returns: string of hash
static char *hashPsk(const unsigned char *ssid, int slen, const unsigned char *psk, int plen)
{
  unsigned char hash[32] = {0};
  char *out = malloc(sizeof(char) * 64);
  int i;

  for (i = 0; i < 64; i++)
    snprintf(&out[i], 2, "%02x", hash[i]);
  return out;
}
