/*
 * Surf API / surf_error.c
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
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

void testErrors()
{
  printf("/*********************************/\n");
  printf("/*         Error Testing         */\n");
  printf("/*********************************/\n\n");

  noInit();
  printf("\n");

  printf("Disregard: connecting to wpa_supplicant for further testing.\n");
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
}
