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
#define DEF_CONFIG_FILE "/etc/wpa_supplicant.conf"
static const char *conf_filepath = DEF_CONFIG_FILE;
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

/**************************
  Helper Functions
**************************/
static int conf_write(const char *dat)
{
  fwrite(dat, sizeof(char), strlen(dat), curr_conf);
  if (ferror(curr_conf)) {
    fprintf(stderr, "wbapi: error writing to configuration file!\n");
    return -1;
  } else if (feof(curr_conf)) {
    fprintf(stderr, "wbapi: end of configuration file reached!\n");
    return -1;
  } else {
    return 0;
  }
}

static void api_exit(const char *msg)
{
  fprintf(stderr, "wbapi error: %s\n", msg);

  if (curr_conf) {
    fclose(curr_conf);
  }
}

/************************
  API Functions
************************/

int api_init()
{
  /* Open default interface directory; look for interface */
  char *iface_dir;
  struct dirent *dent;
  DIR *dir = opendir(ctrl_iface_dir);

  if (dir == NULL) {
    api_exit("Default interface location does not exist!\n");
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
    api_exit("Unable to connect to wpa_supplicant on this interface!\n");
    return -1;
  }

  /* Grab config file */
  if ((curr_conf = fopen(conf_filepath, "r+x")) == NULL) {
    api_exit("Error opening configuration file");
    return -1;
  }
  fprintf(stderr, "Current configuration file: %s\n", conf_filepath);

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

int conf_configAuto(char *ssid, char *psk)
{
  char *line;
  char repl[128];

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  /* check ssid validity */
  
  if (fseek(curr_conf, 0, SEEK_END) < 0) {
    fprintf(stderr, "wbapi: error seeking in configuration file!\n");
    return -1;
  }

  conf_write("\nnetwork={\n");

  sprintf(line, "\tssid=\"%s\"\n", ssid);
  conf_write(line);

  if (psk == NULL) {
    conf_write("\tkey_mgmt=NONE\n");
  } else {
    conf_write("\tkey_mgmt=WPA_PSK\n");
  }

  // psk is having issues?
  /*sprintf(line, "\tpsk=\"%s\"\n", psk);
  conf_write(line);*/

  conf_write("}\n");

  if (wpaReq("RECONFIGURE", 11, repl, 128) < 0) {
    return -1;
  }
  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: error in re-reading configuration file!\n");
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
  char *line;
  char repl[128];

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  if (conf.ssid == NULL) {
    fprintf(stderr, "Cannot create network without ssid!\n");
    return -1;
  }
  /* check ssid validity */

  if (fseek(curr_conf, 0, SEEK_END) < 0) {
    fprintf(stderr, "wbapi: error seeking in configuration file!\n");
    return -1;
  }

  conf_write("\nnetwork={\n");

  /* ssid */
  sprintf(line, "\tssid=\"%s\"\n", conf.ssid);
  conf_write(line);

  /* key_mgmt */
  if (conf.key_mgmt) {
    sprintf(line, "\tkey_mgmt=%s\n", conf.key_mgmt);
    conf_write(line);
  }

  /* psk */
  if (conf.psk) {
    sprintf(line, "\tpsk=\"%s\"\n", conf.psk);
    conf_write(line);
  }

  /* priority */
  if (conf.priority > 0) {
    sprintf(line, "\tpriority=%d\n", conf.priority);
    conf_write(line);
  }

  /* EAP Specific */

  /* identity */
  if (conf.identity) {
    sprintf(line, "\tidentity=\"%s\"\n", conf.identity);
    conf_write(line);
  }

  /* password */
  if (conf.password) {
    sprintf(line, "\tpassword=\"%s\"\n", conf.password);
    conf_write(line);
  }

  /* proto */
  if (conf.proto) {
    sprintf(line, "\tproto=%s\n", conf.proto);
    conf_write(line);
  }

  /* pairwise */
  if (conf.pairwise) {
    sprintf(line, "\tpairwise=%s\n", conf.pairwise);
    conf_write(line);
  }

  /* group */
  if (conf.group) {
    sprintf(line, "\tgroup=%s\n", conf.group);
    conf_write(line);
  }

  /* eap */
  if (conf.eap) {
    sprintf(line, "\teap=%s\n", conf.eap);
    conf_write(line);
  }

  /* phase2 */
  if (conf.phase2) {
    sprintf(line, "\tphase2=\"%s\"\n", conf.phase2);
    conf_write(line);
  }

  conf_write("}\n");

  if (wpaReq("RECONFIGURE", 11, repl, 128) < 0) {
    return -1;
  }
  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: error in re-reading configuration file!\n");
    return -1;
  }

  return 0;
}

int conf_editNetwork(char *ssid, struct wifi_conf conf)
{
  return -1;
}

int conf_deleteNetwork(char *ssid)
{
  return -1;
}

int conf_cleanNetworks(void)
{
  char c;
  int i = 0;
  rewind(curr_conf);

  /* figure out how to remove networks */

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

int listAvailable(char *buf, size_t len)
{
  int retval;

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
