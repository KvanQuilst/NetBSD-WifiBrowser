#include "wbapi.h"

int main(int argc, char **argv)
{
  char buf[4096];
  size_t len;
  int retval;

  if (api_init() == 0) {
    printf("Connected to wpa_supplicant!\n");
  } else {
    printf("Failed to connect to wpa_supplicant...\n");
    exit(1);
  }

  printf("\n");

  len = listAvailable(buf, 4096);

  if (len < 0) {
    return 0;
  }

  printf("List of available networks:\n%s\n", buf);
  printf("Length of networks: %ld\n", len);

  return 0;
}
