#include "wbapi.h"
#define BUF_SIZE 4096

int main(int argc, char **argv)
{
  
  char buf[BUF_SIZE];
  size_t len;
  int retval;

  /* Connect to wpa_supplicant */
  if (api_init() == 0) {
    printf("Connected to wpa_supplicant!\n");
  } else {
    printf("Failed to connect to wpa_supplicant...\n");
    exit(1);
  }
  printf("\n");

  /* List available networks to interface */
  len = listAvailable(buf, BUF_SIZE);

  if (len < 0) {
    return 0;
  }

  printf("List of available networks:\n%s\n", buf);
  printf("Length of networks: %ld\n", len);
  printf("\n");
  
  /* List configured networks visible to wpa_supplicant */
  len = listConfigured(buf, BUF_SIZE);

  printf("List of configured networks:\n%s\n", buf);
  printf("Length of networks: %ld\n", len);
  printf("\n");
  
  /* Create a new network configuration */
  retval = conf_configAuto("test", NULL);
  if (retval < 0) {
    printf("Auto-configuration failed!\n");
  }

  len = listConfigured(buf, BUF_SIZE);
  printf("List of configured networks:\n%s\n", buf);
  printf("\n");

  return 0;
}
