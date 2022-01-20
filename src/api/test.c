#include "wbapi.h"

int main(int argc, char **argv)
{
  char buf[1024] = {0};
  size_t len;

  if (api_init() == 0) {
    printf("Connected to wpa_supplicant!\n");
  } else {
    printf("Failed to connect to wpa_supplicant...\n");
  }

  len = listAvailable(buf);
  printf("List of available networks:\n%s\n", buf);

  return 0;
}
