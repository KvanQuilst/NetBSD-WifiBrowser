/*
 * Surf API / surf_error.c
 * Copyright (c) 2022 Dylan Eskew
 * * This software is under the terms of the BSD license.  * See README for more details.
 *
 * Test for anticipated errors in Surf
 */

#include <stdio.h>
#include <stdlib.h>
#include <surf.h>

static void noInit()
{
  printf("/* wpa_supplicant connection */\n");
  if (conf_configAuto("SSID", NULL) != -1)
    printf("Test failed: No error if surf is not connected to wpa_supplicant\n");
  else
    printf("OK\n");
}

static void nullSSID()
{
  printf("/* no provided ssid */\n");
  if (conf_configAuto(NULL, NULL) != -1)
    printf("Test failed: No error when a NULL ssid is provided to auto-configurator\n");
  else
    printf("OK\n");
}

static void longSSID()
{
  printf("/* provided ssid too long */\n");
  if (conf_configAuto("LookLookLookLookLookLookLookLookLook", NULL) != -1)
    printf("Test failed: No error when ssid is too long!\n");
  else
    printf("OK\n");
}

static void shortPSK()
{
  printf("/* provided psk too short */\n");
  if (conf_configAuto("ValidSSID", "PSK") != -1)
    printf("Test failed: No error when psk is too short!\n");
  else
    printf("OK\n");
}

static void eapNoUser()
{
  printf("/* EAP Auto-config no provided user */\n");
  if (conf_configAutoEAP("SSID", NULL, "PWD") != -1)
    printf("Test failed: No error when no username is provided in EAP config\n");
  else
    printf("OK\n");
}

static void eapNoPWD()
{
  printf("/* EAP Auto-config no provided password */\n");
  if (conf_configAutoEAP("SSID", "USER", NULL) != -1)
    printf("Test failed: No error when no password is provided in EAP config\n");
  else
    printf("OK\n");
}

static void addNoSSID()
{
  printf("/* addEntry no provided SSID */\n");
  if (conf_addEntry(NULL) != -1)
    printf("Test failed: No error when no SSID is provided in addEntry\n");
  else
    printf("OK\n");
}

static void editNoField() {
  printf("/* editNetwork no provided field */\n");
  if (conf_editNetwork("ValidSSID", NULL, "VALUE") != -1)
    printf("Test failed: No error when no field is provided in editNetwork\n");
  else
    printf("OK\n");
}

static void editNoValue() {
  printf("/* editNetwork no provided field value */\n");
  if (conf_editNetwork("ValidSSID", "FIELD", NULL) != -1)
    printf("Test failed: No error when no field is provided in editNetwork\n");
  else
    printf("OK\n");
}

void testErrors()
{
  printf("/*********************************/\n");
  printf("/*         Error Testing         */\n");
  printf("/*********************************/\n\n");

  noInit();
  printf("\n");

  if (surf_init() < 0) {
    printf("Error connecting to wpa_supplicant... Is it running?\n");
    return;
  }
  printf("\n");

  nullSSID();
  printf("\n");

  longSSID();
  printf("\n");

  shortPSK();
  printf("\n");

  eapNoUser();
  printf("\n");

  eapNoPWD();
  printf("\n");

  addNoSSID();
  printf("\n");

  editNoField();
  printf("\n");

  editNoValue();
  printf("\n");
}
