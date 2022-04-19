/*
 * Surf API / surf_error.c
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * * This software is under the terms of the BSD license.  * See README for more details.
 *
 * Test for anticipated errors in Surf
 */

#include "surf.h"

void no_init()
{
  if (conf_configAuto("SSID", "PSK") != -1)
    printf("Test failed: No error if surf is not connected to wpa_supplicant\n");
  else
    printf("OK\n");
}

int main()
{
  no_init();

  if (surf_init() < 0) {
    printf("Error connecting to wpa_supplicant... Is it running?\n");
    return 1;
  }



  return 0;
}
