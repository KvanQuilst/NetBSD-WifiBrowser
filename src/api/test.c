#include "wbapi.h"
#define BUF_SIZE 4096

int main(int argc, char **argv)
{
  
  char buf[BUF_SIZE];
  int len;
  int retval;

  /* Connect to wpa_supplicant */
  printf("/* Connect to wpa_supplicant */\n");
  if (api_init() == 0) {
    printf("Connected to wpa_supplicant!\n");
  } else {
    printf("Failed to connect to wpa_supplicant...\n");
    exit(1);
  }
  printf("\n");

  /* List available networks to interface */
  printf("/* List available networks */\n");
  len = listAvailable(buf, BUF_SIZE);

  if (len < 0) {
    return 0;
  }

  printf("List of available networks:\n%s\n", buf);
  printf("Length of networks: %d\n", len);
  printf("\n");
  
  /* List configured networks visible to wpa_supplicant */
  printf("/* List configured networks */");
  len = listConfigured(buf, BUF_SIZE);

  printf("List of configured networks:\n%s\n", buf);
  printf("Length of networks: %d\n", len);
  printf("\n");
  
  /* Create a new network configuration */
  printf("/* Auto configure network w/ ssid */\n");
  retval = conf_configAuto("Auto-SSID", NULL);
  if (retval < 0) {
    printf("Auto-configuration failed!\n");
  }

  len = listConfigured(buf, BUF_SIZE);
  printf("List of configured networks:\n%s\n", buf);
  printf("\n");

  /* Manual configuration WPA-PSK network */
  printf("/* Manually configure network w/ ssid */\n");
  struct wifi_conf w;
  w.ssid = "Manual-PSK";
  w.key_mgmt = "NONE";
  w.priority = 1;

  retval = conf_configManual(w);
  if (retval < 0) {
    printf("Manual configuration failed!\n");
  }

  len = listConfigured(buf, BUF_SIZE);
  printf("List of configure networks:\n%s\n", buf);
  printf("\n");

  /* Manual configuratino WPA-EAP network */
  printf("/* Manually configure network with WPA-EAP data */\n");
  struct wifi_conf e;
  e.ssid = "Manual-EAP";
  e.key_mgmt = "WPA-EAP";
  e.proto = "RSN";
  e.pairwise = "CCMP";
  e.group = "CCMP";
  e.eap = "PEAP";
  e.identity = "royd4";
  e.password = "pass";
  e.phase2="auth=MSCHAPV2";

  retval = conf_configManual(e);
  if (retval < 0) {
    printf("Manual EAP configuration failed!\n");
  }

  len = listConfigured(buf, BUF_SIZE);
  printf("List of configured networks:\n%s\n", buf);
  printf("\n");

  return 0;
}
