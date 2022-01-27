#include "wbapi.h"

int main(int argc, char **argv)
{
  char buf[4096];
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
  len = listAvailable(buf, 4096);

  if (len < 0) {
    return 0;
  }

  printf("List of available networks:\n%s\n", buf);
  printf("Length of networks: %ld\n", len);
  printf("\n");
  
  /* List configured networks visible to wpa_supplicant */
  len = listConfigured(buf, 4096);

  printf("List of configured networks:\n%s\n", buf);
  printf("Length of networks: %ld\n", len);
  printf("\n");
  

  return 0;
}
