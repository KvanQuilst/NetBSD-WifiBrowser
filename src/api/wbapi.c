/*
 * Wifi Browser API / wbapi.c
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * * This software is under the terms of the BSD license.  * See README for more details.
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

#define MAX_SSID_LEN 32


/**************************
  Static Prototypes
**************************/
static FILE *conf_create(const char *filepath);
static char *hashPsk(const unsigned char *ssid, int slen, const unsigned char *psk, 
    int plen);
//static char *hashPwd(char *pwd, int plen); 
//static void getKeyMgmt(char *ssid, wifi_conf *conf);

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

static int reconfigure()
{
  char repl[128] = {0};

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }
  
  if (wpaReq("RECONFIGURE", 11, repl, 128) < 0) {
    return -1;
  }
  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: error in re-reading configuration file!\n");
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
    fprintf(stderr, "wbapi: error in saving configuration!\n");
    return -1;
  }
  return reconfigure();
}

static int getNetworkID(const char *ssid)
{
  int id = 0;
  char cmd[32] = {0};
  char repl[128] = {0};

  while(strncmp(repl, "FAIL", 4) && id < 4) {
    sprintf(cmd, "GET_NETWORK %d ssid", id);
    if (wpaReq(cmd, sizeof(cmd), repl, 128) < 0) {
      fprintf(stderr, "wbapi: error in obtaining network id\n");
      return -1;
    }
    sscanf(repl, "\"%[^\",]\"", repl);
    if (!strncmp(ssid, repl, MAX_SSID_LEN)) {
      return id;
    }
    id++;
  }

  fprintf(stderr, "wbapi: network ssid does not exist!\n");
  return -1;
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

/*int conf_setCurrent(const char *filepath)
{
  return -1;
}*/

int conf_configAuto(const char *ssid, const char *psk)
{
  //char line[128] = {0};
  char repl[128] = {0};
  char cmd[128] = {0};
  char *hash;
  int id, plen, slen;

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  /* check ssid validity */
  if (ssid == NULL) {
    fprintf(stderr, "wbapi: no ssid provided\n");
    return -1;
  }

  slen = strnlen(ssid, 33);
  if (slen == 0 || slen > 32) {
    fprintf(stderr, "wbapi: provided ssid is invalid. ssid length: 1-32 characters\n");
    return -1;
  }

  if (wpaReq("ADD_NETWORK", 11, repl, sizeof(repl)) < 0)
    return -1;
  if (!strncmp("FAIL", repl, 4)) {
    fprintf(stderr, "wbapi: failed to add new network\n");
    return -1;
  }
  id = atoi(repl);

  snprintf(cmd, sizeof(cmd), "SET_NETWORK %d ssid \"%s\"", id, ssid);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: failed to set ssid %s\n", ssid);
    return -1;
  }

  if (psk == NULL)
    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d key_mgmt NONE", id);
  else {
    plen = strnlen(psk, 64);
    if (plen < 8 || plen > 63) {
      fprintf(stderr, "wbapi: provided passkey is invalid. passkey length: 8-63 characters\n");
      return -1;
    }
    hash = hashPsk(ssid, slen, psk, plen);

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d key_mgmt NONE", id);
    if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
      return -1;
    if (strncmp("OK", repl, 2)) {
      fprintf(stderr, "wbapi: failed to set key_mgmt\n");
      return -1;
    }

    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d psk %s", id, hash);
  }

  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: failed to set field\n");
    return -1;
  }
  
  /*if (fseek(curr_conf, 0, SEEK_END) < 0) {
    fprintf(stderr, "wbapi: error seeking in configuration file!\n");
    return -1;
  }

  conf_write("\nnetwork={\n");

  sprintf(line, "\tssid=\"%s\"\n", ssid);
  conf_write(line);

  if (psk == NULL) {
    conf_write("\tkey_mgmt=NONE\n");
  } else {
    plen = strnlen(psk, 64);
    if (plen < 8 || plen > 63) {
      fprintf(stderr, "wbapi: provided passkey is invalid. passkey length: 8-63 characters\n");
      return -1;
    }
    hash = hashPsk(ssid, slen, psk, plen);

    conf_write("\tkey_mgmt=WPA-PSK\n");
    sprintf(line, "\tpsk=%s\n", hash);
    conf_write(line);
  }

  conf_write("}\n");

  fflush(curr_conf);*/

  return save();
}

/*int conf_configAutoEAP(const char *ssid, const char *user, const char *pwd)
{
  return -1;
}*/

int conf_addEntry(const char *ssid)
{
  char repl[128] = {0};
  char cmd[128] = {0};
  int id, slen;

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  /* check ssid validity */
  if (ssid == NULL) {
    fprintf(stderr, "wbapi: no ssid provided\n");
    return -1;
  }

  slen = strnlen(ssid, 33);
  if (slen == 0 || slen > 32) {
    fprintf(stderr, "wbapi: provided ssid is invalid. ssid length: 1-32 characters\n");
    return -1;
  }

  if (wpaReq("ADD_NETWORK", 11, repl, sizeof(repl)) < 0)
    return -1;
  if (!strncmp("FAIL", repl, 4)) {
    fprintf(stderr, "wbapi: error in adding new network\n");
    return -1;
  }
  id = atoi(repl);

  snprintf(cmd, sizeof(cmd), "SET_NETWORK %d ssid \"%s\"", id, ssid);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: error in setting network ssid: %s\n", ssid);
    return -1;
  }

  snprintf(cmd, sizeof(cmd), "SET_NETWORK %d key_mgmt NONE", id);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: error in setting network key_mgmt\n");
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
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  /* verify ssid */
  if (ssid == NULL) {
    fprintf(stderr, "wbapi: no ssid provided\n");
    return -1;
  }

  slen = strnlen(ssid, 33);
  if (slen == 0 || slen > 32) {
    fprintf(stderr, "wbapi: provided ssid is invalid. ssid length: 1-32 characters\n");
    return -1;
  }

  id = getNetworkID(ssid);
  if (id < 0)
    return -1;

  snprintf(cmd, sizeof(cmd), "DISABLE_NETWORK %d", id);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: error in disabling network %s\n", ssid);
    return -1;
  }

  if (!strncmp(field, "psk", 4)) {
    plen = strnlen(value, 64);
    if (plen < 8 || plen > 63) {
      fprintf(stderr, "wbapi: provided passkey is invalid. passkey length: 8-63 characters\n");
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
    fprintf(stderr, "wbapi: error in setting network field: %s\n", field);
    return -1;
  }
  return 0;
}

int conf_enableNetwork(const char *ssid)
{
  char repl[128] = {0};
  char cmd[32] = {0};
  int id, slen;

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  /* verify ssid */
  if (ssid == NULL) {
    fprintf(stderr, "wbapi: no ssid provided\n");
    return -1;
  }

  slen = strnlen(ssid, 33);
  if (slen == 0 || slen > 32) {
    fprintf(stderr, "wbapi: provided ssid is invalid. ssid length: 1-32 characters\n");
    return -1;
  }

  id = getNetworkID(ssid);
  if (id < 0)
    return -1;

  snprintf(cmd, sizeof(cmd), "ENABLE_NETWORK %d", id);
  if (wpaReq(cmd, sizeof(cmd), repl, sizeof(repl)) < 0)
    return -1;
  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: error in enabling network %s\n", ssid);
    return -1;
  }
  return save();
}

int conf_deleteNetwork(const char *ssid)
{
  char repl[128] = {0};
  char cmd[32] = {0};
  int id;

  /* verify ssid */
  if (ssid == NULL) {
    fprintf(stderr, "wbapi: no ssid provided\n");
    return -1;
  }

  id = getNetworkID(ssid);
  if (id < 0)
    return -1;

  sprintf(cmd, "REMOVE_NETWORK %d", id);
  if (wpaReq(cmd, sizeof(cmd), repl, 128) < 0)
    return -1;

  if (strncmp("OK", repl, 2)) {
    fprintf(stderr, "wbapi: error in removing network: %s\n", ssid);
    return -1;
  }

  return save();
}

int conf_cleanNetworks(void)
{
  fclose(curr_conf);
  remove(conf_filepath);
  curr_conf = conf_create(conf_filepath);
  return reconfigure();
}

int listConfigured(char *buf, size_t len)
{
  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  return wpaReq("LIST_NETWORKS", 13, buf, len);
}

int listAvailable(char *buf, size_t len)
{
  //char list[512] = {0};
  //char currSSID[34] = {0};

  if (!wpa) {
    fprintf(stderr, "Not connected to wpa_supplicant...\n");
    return -1;
  }

  if (wpaReq("SCAN", 4, buf, len) < 0) return -1;

  if (wpaReq("SCAN_RESULTS", 12, buf, len) < 0) return -1;

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
// requires: string of ssid, length of ssid, string of passkey, length of passkey
// returns: string of hash
static char *hashPsk(const unsigned char *ssid, int slen, const unsigned char *psk, int plen)
{
  unsigned char hash[32] = {0};
  char *out = malloc(sizeof(char) * 64);
  int i;

  PKCS5_PBKDF2_HMAC_SHA1(psk, plen, ssid, slen, 4096, 32, hash);
  for (i = 0; i < 64; i++)
    snprintf(&out[i], 2, "%02x", hash[i]);
  return out;
}

// hash a password using openssl for use in
// a configuration file
// requires: string of password
// returns: string of hashed password
/*static char *hashPwd(char *pwd, int plen)
{
  unisgned char hash[  
  return NULL;
}*/

// determines the key management protocol of the
// specified ssid and updates it in the configuration struct
// requires: string of ssid, reference to wifi_conf struct of interest
/*static void getKeyMgmt(char *ssid, wifi_conf *conf)
{
  return;
}*/


/*int wpa_restart()
{
  return -1;
}*/

/*int wpa_running()
{
  return 0;
}*/
