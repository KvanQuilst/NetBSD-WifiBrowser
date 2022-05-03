/*	$NetBSD: $	*/

/*
 * Copyright (c) 2022 Philip A. Nelson.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Philip A. Nelson.
 * 4. The name of Philip A. Nelson may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY PHILIP NELSON ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL PHILIP NELSON BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "extern.h"
#include "surf.h" 

/* 
 * Function PROCS to be used with program 
 */
CMD_PROC (do_exit);
CMD_PROC (conf);
CMD_PROC (ls);

/*
 * TODO: Implement checks for each method (check correct number of arguments)  
 * TODO: Allow user to enter a string of commands to be processed all at once  
 * Note: Not sure how to use getopt_long without editing command.c 
 * Note: Set a variable to args and increment with each command entered 
*/

int conf(int num, char **args, char *syntax){

  if(num < 2){
    
    printf("Requires at least two additional arguments.\n");
  }

  else if(strcmp(args[1], "-a") == 0){

    if(num < 4){

      printf("Auto configuration requires ssid, psk.\n");
    }
    
    else{

      if(conf_configAuto(args[2], args[3]) < 0){

        printf("Error setting auto configuration of network.\n");
      }
      else{

        printf("Success setting auto configuration of network.\n");
      }
    }
  }

  else if(strcmp(args[1], "-ae") == 0){

    if(num < 5){

      printf("Auto configuration eap requires ssid, user and pwd.\n");
    }

    else{

      if(conf_configAutoEAP(args[2], args[3], args[4]) < 0){

        printf("Error setting auto configuration eap of network.\n");
      }

      else{

        printf("Success setting auto configuration eap of network.\n");
      }
    }
  }

  else if(strcmp(args[1], "-an") == 0){

    if(num < 3){

      printf("Adding network requires ssid.\n");
    }

    else{

      if(conf_addEntry(args[2]) < 0){

        printf("Error adding new entry.\n");
      }

      else{

        printf("Type 'n' to end additional fields for new network.\n");
        while(TRUE){

          char field[FIELDLEN], value[FIELDLEN]; 
          printf("field: "); fgets(field, FIELDLEN, stdin); 
          printf("value: "); fgets(value, FIELDLEN, stdin); 
          field[strlen(field) - 1] = 0; 
          value[strlen(value) - 1] = 0; 

          if(strcmp(field, "n") == 0 || strcmp(value, "n") == 0){

            break;
          }

          else{

            if(conf_editNetwork(args[2], field, value) < 0){

              printf("Error editing specified network field to configuration file.\n"); 
            }
            
            else{

              printf("Field added to configuration file.\n");
            }
          }
        }

        if(conf_enableNetwork(args[2]) < 0){

          printf("Error enabling new network in configuration file.\n");
        }

        else{

          printf("Success enabling new network in configuration file.\n");
        }
      }
    }
  }

  else if(strcmp(args[1], "-en") == 0){

    if(num < 3){

      printf("Enabling network requires ssid.\n");
    }

    else{

      if(conf_enableNetwork(args[2]) < 0){

        printf("Error enabling network.\n");
      }

      else{

        printf("Success enabling network.\n");
      }
    }
  }

  else if(strcmp(args[1], "-dn") == 0){

    if(num < 3){

      printf("Deleting network requires ssid.\n");
    }

    else{

      if(conf_deleteNetwork(args[2]) < 0){

        printf("Error deleting network.\n");
      }
      
      else{

        printf("Success deleting network.\n");
      }
    }
  }

  return 0;
}

int ls(int num, char **args, char *syntax){

  if(num < 2){

    printf("Requires at least two additional arguments.\n");
  }

  else{

    char buffer[BUFSIZ];
    if(strcmp(args[1], "-c") == 0){

      if(listConfigured(buffer, BUFSIZ) < 0){

        printf("Error listing configured networks.\n");
      }

      else{

        printf("%s\n", buffer);
      }
    }

    if(strcmp(args[1], "-a") == 0){

      if(listAvailable(buffer, BUFSIZ) < 0){

        printf("Error listing available networks.\n");
      }

      else{

        printf("%s\n", buffer);
      }
    }
  }

  return 0;
}

int editNetwork(char *ssid, char *field, char *value){

  if(conf_editNetwork(ssid, field, value) < 0){

    printf("Field %s for %s has been changed. \n", field, value);
    return 1;
  }

  else{

    printf("Error editing specified network in configuration file. \n");    
    return 0;
  }
}

int do_exit(int num, char **args, char *syntax){

  return 0;
}

/* Main program */
int main (int argc, char **argv) {
  
  /* Initiate API at start of program */
  if(surf_init() < 0){

    printf("Error connecting to wpa_supplicant.\n");
  }
  
  else{

    printf("Success connecting to wpa_supplicant.\n");
  }

  /* Run command loop */
  command_loop();
}
