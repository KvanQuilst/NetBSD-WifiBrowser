/*
 * Wifi Browser API / test.c
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * * This software is under the terms of the BSD license.  * See README for more details.
 *
 * Central file for testing wbapi
 */

#include "wbapi.h"
#define BUF_SIZE 4096

const char *net1 = "Auto-SSID";
const char *net2 = "Manual-PSK";
const char *net3 = "Manual-EAP";

#ifdef DEBUG
static void printConfig()
{
  char buf[BUF_SIZE];

  if (listConfigured(buf, BUF_SIZE) >= 0)
    printf("List of configured networks:\n%s\n", buf);
}
#endif

static void test_connect()
{
  printf("/* Connect to wpa_supplicant */\n");
  if (api_init() == 0)
    printf("OK\n");
  else {
    printf("Failed to connect to wpa_supplicant...\n");
    exit(1);
  }
  printf("\n");
}

static void test_cleanConfig()
{
  printf("/* Clean configuration file for testing */\n");
  if (conf_cleanNetworks() < 0)
    printf("Failed to clean all networks from config file!\n");
  else
    printf("OK\n");
  printf("\n");
}

static void test_listAvailable()
{
  char buf[BUF_SIZE];

  printf("/* List available networks */\n");

  if (listAvailable(buf, BUF_SIZE) < 0)
    printf("List available failed!\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printf("List of available networks:\n%s\n", buf);
#endif
  printf("\n");
}

static void test_listConfigured()
{
  char buf[BUF_SIZE];

  printf("/* List configured networks */\n");
  if (listConfigured(buf,BUF_SIZE) < 0)
    printf("List configured failed!\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printf("List of configured networks:\n%s\n", buf);
#endif
  printf("\n");
}

static void test_autoConf()
{
  printf("/* Auto configure network w/ ssid */\n");
  if (conf_configAuto(net1, "Password") < 0)
    printf("Auto-configuration failed!\n");
  else
    printf("OK\n");
#ifdef DEBUG
  printConfig();
#endif
  printf("\n");
}

static void test_manualConf()
{
  wifi_conf w = wc_init();

  printf("/* Manually configure network w/ ssid */\n");
  w.ssid = net2;
  w.key_mgmt = "WPA-PSK";
  w.psk = "Password2";
  w.priority = 1;

  if (conf_configManual(w) < 0)
    printf("Manual configuration failed!\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printConfig();
#endif
  printf("\n");
}

static void test_manualEAP()
{
  wifi_conf e = wc_init();

  printf("/* Manually configure network with WPA-EAP data */\n");
  e.ssid = net3;
  e.key_mgmt = "WPA-EAP";
  e.proto = "RSN";
  e.pairwise = "CCMP";
  e.group = "CCMP";
  e.eap = "PEAP";
  e.identity = "royd4";
  e.password = "pass";
  e.phase2="auth=MSCHAPV2";

  if (conf_configManual(e) < 0)
    printf("Manual EAP configuration failed!\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printConfig();
#endif
  printf("\n");
}

static void test_deletion() {
  int retval;

  printf("/* Delete network %s*/\n", net1);
  retval = conf_deleteNetwork(net1);
  if (retval < 0)
    printf("Deletion failed!\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printConfig();
#endif
  printf("\n");
}

static void test_edit() {
  int retval;

  printf("/* Edit network %s*/\n", net2);
  retval = conf_editNetwork(net2, "ssid", "editNetwork");
  if (retval < 0)
    printf("Edit network failed\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printConfig();
#endif
  printf("\n");
}

int main()
{
  /* Connect to wpa_supplicant */
  test_connect();

  /* Clean configuration file for testing */
  test_cleanConfig();

  /* List available networks to interface */
  test_listAvailable();

  /* List configured networks visible to wpa_supplicant */
  test_listConfigured();
  
  /* Create a new network configuration */
  test_autoConf();

  /* Manual configuration WPA-PSK network */
  test_manualConf();

  /* Manual configuration WPA-EAP network */
  test_manualEAP();

  /* Deletion of a network */
  test_deletion();

  /* Editting of a network */
  test_edit();

  return 0;
}
