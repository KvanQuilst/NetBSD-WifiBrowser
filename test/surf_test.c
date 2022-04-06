/*
 * Surf API / surf_test.c
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * * This software is under the terms of the BSD license.  * See README for more details.
 *
 * Test file for the Surf API
 */

#include <stdio.h>
#include <stdlib.h>

#include "surf.h"
#define BUF_SIZE 4096

const char *net1 = "Auto-SSID";
const char *net2 = "Manual-PSK";
const char *net3 = "Manual-EAP";
const char *net4 = "editNetwork";
const char *net5 = "Auto-EAP";
const char *net6 = "Insecure";

#ifdef DEBUG
static void printConfig()
{
  char buf[BUF_SIZE];

  if (listConfigured(buf, BUF_SIZE) >= 0)
    printf("List of configured networks:\n%s", buf);
}
#endif

static void test_connect()
{
  printf("/* Connect to wpa_supplicant */\n");
  if (surf_init() == 0)
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
  printf("List of available networks:\n%s", buf);
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

static void test_autoConfPSK()
{
  printf("/* Auto configure PSK-protected network */\n");
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
  printf("/* Manually configure network w/ ssid */\n");

  if (conf_addEntry(net2) < 0)
    printf("Manual entry failed: ssid\n");
  else if (conf_editNetwork(net2, "key_mgmt", "WPA-PSK") < 0)
    printf("Manual entry failed: key_mgmt\n");
  else if (conf_editNetwork(net2, "psk", "Password2") < 0)
    printf("Manual entry failed: psk\n");
  else if (conf_editNetwork(net2, "priority", "1") < 0)
    printf("Manual entry failed: priority\n");
  else if (conf_enableNetwork(net2) < 0)
    printf("Manual entry failed: enable network\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printConfig();
#endif
  printf("\n");
}

static void test_manualEAP()
{
  printf("/* Manually configure network with WPA-EAP data */\n");

  if (conf_addEntry(net3) < 0)
    printf("Manual EAP configuration failed: add entry\n");
  else if (conf_editNetwork(net3, "key_mgmt", "WPA-EAP") < 0)
    printf("Manual EAP configuration failed: key_mgmt\n");
  else if (conf_editNetwork(net3, "proto", "RSN") < 0)
    printf("Manual EAP configuration failed: proto\n");
  else if (conf_editNetwork(net3, "pairwise", "CCMP") < 0)
    printf("Manual EAP configuration failed: pairwise\n");
  else if (conf_editNetwork(net3, "group", "CCMP") < 0)
    printf("Manual EAP configuration failed: group\n");
  else if (conf_editNetwork(net3, "eap", "PEAP") < 0)
    printf("Manual EAP configuration failed: eap\n");
  else if (conf_editNetwork(net3, "identity", "royd4") < 0)
    printf("Manual EAP configuration failed: identity\n");
  else if (conf_editNetwork(net3, "password", "pass") < 0)
    printf("Manual EAP configuration failed: password\n");
  else if (conf_editNetwork(net3, "phase2", "\"auth=MSCHAPV2\"") < 0)
    printf("Manual EAP configuration failed: phase2\n");
  else if (conf_enableNetwork(net3) < 0)
    printf("Manual EAP configuration failed: enable\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printConfig();
#endif
  printf("\n");
}

static void test_deletion() {
  printf("/* Delete network %s*/\n", net1);
  if (conf_deleteNetwork(net4) < 0)
    printf("Deletion failed!\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printConfig();
#endif
  printf("\n");
}

static void test_edit() {
  printf("/* Edit network %s*/\n", net1);
  if (conf_editNetwork(net1, "ssid", net4) < 0)
    printf("Edit network failed: edit\n");
  else if (conf_enableNetwork(net4) < 0)
    printf("Edit network failed: enable\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printConfig();
#endif
  printf("\n");
}

void test_autoConfInsec()
{
  printf("/* Auto configure unprotected network */\n");
  if (conf_configAuto(net6, NULL) < 0)
    printf("Auto configure unprotected network failed\n");
  else
    printf("OK\n");

#ifdef DEBUG
  printConfig();
#endif
  printf("\n");
}

void test_autoConfEAP()
{
  printf("/* Auto configure EAP Network */\n");
  if (conf_configAutoEAP(net5, "user", "password") < 0)
    printf("Auto configure EAP network failed\n");
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

  /* Auto Configure an insecure network */
  test_autoConfInsec();
  
  /* Auto Configure a WPA-PSK network */
  test_autoConfPSK();

  /* Auto Configure a WPA-EAP network */
  test_autoConfEAP();

  /* Editting of a network */
  test_edit();

  /* Manual configuration WPA-PSK network */
  test_manualConf();

  /* Manual configuration WPA-EAP network */
  test_manualEAP();

  /* Deletion of a network */
  test_deletion();

  return 0;
}
