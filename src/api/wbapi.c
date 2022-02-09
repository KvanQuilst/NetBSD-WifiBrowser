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
static FILE *conf_create(const char *filepath);
static int wpaReq(const char *cmd, size_t cmd_len, char *repl, size_t repl_len);
static char *hashPsk(char *ssid, char *psk);
static char *hashPwd(char *pwd); 
static void getKeyMgmt(char *ssid, wifi_conf *conf);

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

static int reconfigure()
{
  char repl[128];
  
  if (wpaReq("RECONFIGURE", 11, repl, 128) < 0) {
    return -1;
  }
  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: error in re-reading configuration file!\n");
    return -1;
  }
  return 0;
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

wifi_conf wc_init() {
  wifi_conf w;
  w.ssid = NULL;
  w.psk = NULL;
  w.key_mgmt = NULL;
  w.priority = 0;
  w.identity = NULL;
  w.password = NULL;
  w.proto = NULL;
  w.pairwise = NULL;
  w.group = NULL;
  w.eap = NULL;
  w.phase2 = NULL;
  return w;
}

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
      fprintf(stderr, "Current interface: %s\n", ifname ? ifname : "n/a");
    }
  }

  if (ifname == NULL) {
    api_exit("Default interface location does not exist!\n");
    return -1;
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

static FILE *conf_create(const char *filepath)
{
  FILE *fp;
  time_t t = time(NULL);

  if ((fp = fopen(filepath, "r")) != NULL) {
    fprintf(stderr, "wbapi: Config file already exists - cannot initialize!\n");
    return NULL;
  }

  if ((fp = fopen(filepath, "w+x")) == NULL) {
    fprintf(stderr, "wbapi: Error creating config file\n");
    return NULL;
  }

  fprintf(fp, "# wpa_supplicant configuration file - auto-created by wbapi at %s", ctime(&t));
  fprintf(fp, "ctrl_interface=DIR=%s GROUP=wheel\n", ctrl_iface_dir);
  fprintf(fp, "update_config=1\n");

  fflush(fp);

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
  int len;

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
    len = strnlen(psk, 64);
    if (len < 8 || len > 63) {
      fprintf(stderr, "wbapi: provided passkey is invalid. passkey length: 8-63 characters\n");
      return -1;
    }

    conf_write("\tkey_mgmt=WPA-PSK\n");
    sprintf(line, "\tpsk=\"%s\"\n", psk);
    conf_write(line);
    //hashPsk(ssid, psk);
  }

  conf_write("}\n");

  fflush(curr_conf);

  return reconfigure();
}

int conf_configAutoEAP(char *ssid, char *user, char *pwd)
{
  return -1;
}

int conf_configManual(wifi_conf conf)
{
  char *line;

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

  fflush(curr_conf);

  return reconfigure();
}

int conf_editNetwork(char *ssid, wifi_conf conf)
{
  return -1;
}

int conf_deleteNetwork(char *ssid)
{
  return -1;
}

int conf_cleanNetworks(void)
{
  int i = 0;
  /*char cmd[32] = {0};
  char repl[128] = {0};

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  while (strncmp("FAIL", repl, 4)) {
    snprintf(cmd, 32, "REMOVE_NETWORK %d", i);
    if(wpaReq(cmd, 32, repl, 128) < 0) {
      return -1;
    }
    i++;
  }

  if (wpaReq("SAVE_CONFIG", 11, repl, 128) < 0) {
    return -1;
  }
  if (strncmp("OK", repl, 2)) {
    return -1;
  }*/

  fclose(curr_conf);
  remove(conf_filepath);
  curr_conf = conf_create(conf_filepath);

  return reconfigure();
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
  int retval, i = 0;
  char list[512] = {0};
  char currSSID[34] = {0};

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  if (wpaReq("SCAN", 4, buf, len) < 0) return -1;
  retval = wpaReq("SCAN_RESULTS", 12, buf, len);

  sscanf(buf, "bssid / frequency / signal level / flags / ssid\n%[^\0]", buf); 
  //printf("\nscan:\n%s\n", buf);

  /* isolate ssid for list */
  /*do {
    sscanf(buf, "%*s\t%*s\t%*s\t%*s\t%32[^\n]\n", currSSID);
    printf("currSSID: %s\n", currSSID);
    strncpy(&list[i], currSSID, 32);
    i += strnlen(currSSID, 32);
    list[i] = '\n';
    i++;
    sscanf(buf, "%*[^\n]\n%[^\0]", buf);
  } while (strncmp("", currSSID, 1));

  printf("list: %s\n", list);

  buf = strndup(list, 512);*/

  return 0;
}

// hash a passkey against the associated ssid for 
// use in a configuration file
// DOES NOT AUTOMATICALLY INSERT INTO CONFIG FILE
// requires: string of ssid, string of passkey
// returns: string of hashed passkey
static char *hashPsk(char *ssid, char *psk)
{
  char *line;
  char repl[128];
  sprintf(line, "WPA_CTRL_RSP-PASSPHRASE-%s:%s", ssid, psk);
  if (wpaReq(line, strlen(line), repl, 128) < 0) return -1;
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
// specified ssid and updates it in the configuration struct
// requires: string of ssid, reference to wifi_conf struct of interest
static void getKeyMgmt(char *ssid, wifi_conf *conf)
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
