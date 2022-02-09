#include "wbapi.h"
#define BUF_SIZE 4096

void test_connect()
{
  printf("/* Connect to wpa_supplicant */\n");
  if (api_init() == 0) {
    printf("Connected to wpa_supplicant!\n");
  } else {
    printf("Failed to connect to wpa_supplicant...\n");
    exit(1);
  }
  printf("\n");
}

void test_cleanConfig()
{
  printf("/* Clean configuration file for testing */\n");
  if (conf_cleanNetworks() < 0) {
    printf("Failed to clean all networks from config file!\n");
  } else {
    printf("Configuration file cleaning successful!\n");
  }
  printf("\n");
}

void test_listAvailable()
{
  int len;
  char buf[BUF_SIZE];

  printf("/* List available networks */\n");
  len = listAvailable(buf, BUF_SIZE);

  if (len < 0) {
    return 0;
  }

  printf("List of available networks:\n%s\n", buf);
  printf("\n");
}

void test_listConfigured()
{
  int len;
  char buf[BUF_SIZE];

  printf("/* List configured networks */\n");
  len = listConfigured(buf, BUF_SIZE);

  printf("List of configured networks:\n%s\n", buf);
  printf("\n");
}

void test_autoConf()
{
  int retval, len;
  char buf[BUF_SIZE];

  printf("/* Auto configure network w/ ssid */\n");
  retval = conf_configAuto("Auto-SSID", "Password");
  if (retval < 0) {
    printf("Auto-configuration failed!\n");
  }

  len = listConfigured(buf, BUF_SIZE);
  printf("List of configured networks:\n%s\n", buf);
  printf("\n");
}

void test_manualConf()
{
  wifi_conf w = wc_init();
  int retval, len;
  char buf[BUF_SIZE];

  printf("/* Manually configure network w/ ssid */\n");
  w.ssid = "Manual-PSK";
  w.key_mgmt = "WPA-PSK";
  w.psk = "Password2";
  w.priority = 1;

  retval = conf_configManual(w);
  if (retval < 0) {
    printf("Manual configuration failed!\n");
  }

  len = listConfigured(buf, BUF_SIZE);
  printf("List of configure networks:\n%s\n", buf);
  printf("\n");
}

void test_manualEAP()
{
  wifi_conf e = wc_init();
  int retval, len;
  char buf[BUF_SIZE];

  printf("/* Manually configure network with WPA-EAP data */\n");
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

  /* Manual configuratino WPA-EAP network */
  test_manualEAP();

  return 0;
}
