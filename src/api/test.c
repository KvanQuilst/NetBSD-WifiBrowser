#include "wbapi.h"

int main(int argc, char **argv)
{

  if (api_init() == 0) {
    printf("Connected to wpa_supplicant!\n");
  } else {
    printf("Failed to connect to wpa_supplicant...\n");
  }
  
  return 0;
}
