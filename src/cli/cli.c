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
CMD_PROC (disconnect);
CMD_PROC (configure);
CMD_PROC (current);
CMD_PROC (do_exit);
CMD_PROC (forget);
CMD_PROC (conn);
CMD_PROC (conf);
CMD_PROC (edit);
CMD_PROC (list);
CMD_PROC (add);

/* Connect to an already existing network */
int conn(int num, char **args, char *syntax){

  if(num < 2){

     printf("%s", syntax);
  }

  else{

    if(conf_enableNetwork(args[1]) < 0){

      printf("Error: Could not connect to network %s\n", args[1]);
    }

    else{

      printf("Connected to network %s successfully!\n", args[1]);
    }
  }

  return 0;
}

/* Disconnect from an already exisiting connection */
int disconnect(int num, char **args, char *syntax){

  if(num < 2){

    printf("%s", syntax);
  }

  else{

    char *current = malloc(sizeof(char) * 32);
    strncpy(current, currConnection(), sizeof(char) * 32);
    if(current != NULL){

      if(conf_disableNetwork(current) < 0){

        printf("Error: Could not disconnect from netowrk %s\n", args[1]);
      }

      else{

        printf("Disconnected from network %s successfully!\n", args[1]);
      }
    }

    else{

      printf("Not connected to a network!\n");
    }

    free(current);
  }

  return 0;
}

/* Add a new network to the configuration file */
int add(int num, char **args, char *syntax){

  if(num < 2){

    printf("%s", syntax);
  }

  else{

    if(conf_addEntry(args[1]) < 0){

      printf("Error: Could not add new network connection %s\n", args[1]);
    }
    
    else{

      printf("Network %s added successfully!\n", args[1]);
    }
  }

  return 0;
}

/* Forget a network in the configuration file */
int forget(int num, char **args, char *syntax){

   if(num < 2){

     printf("%s", syntax);
  }

  else{

    if(conf_deleteNetwork(args[1]) < 0){

      printf("Error: Could not remove network %s from configuration file.\n", args[1]);
    }

    else{

      printf("Network %s removed successfully!\n", args[1]);
    }
  }

  return 0;
}

/* Edit a network in the configuration file */
int edit(int num, char **args, char *syntax){

  if(num < 2){

    printf("%s", syntax);
  }

  else{

    if(num < 4){
      char field[FIELDLEN], value[FIELDLEN]; 
      printf("field: "); fgets(field, FIELDLEN, stdin); 
      printf("value: "); fgets(value, FIELDLEN, stdin); 
      field[strlen(field) - 1] = 0; 
      value[strlen(value) - 1] = 0; 

      if(conf_editNetwork(args[1], field, value) < 0){

        printf("Error: Could not edit specified field in network %s\n" 
                  "field: %s\nvalue: %s\n", args[1], field, value); 
      }

      else{

         printf("Success editing specified field to network %s!\n"
                "field: %s\nvalue: %s\n", args[1], field, value);
      }
    }

    else{

      if(conf_editNetwork(args[1], args[2], args[3]) < 0){

        printf("Error: Could not edit specified field in network %s\n" 
                  "field: %s\nvalue: %s\n", args[1], args[2], args[3]); 
      }

      else{

        printf("Success editing specified field to network %s!\n"
                "field: %s\nvalue: %s\n", args[1], args[2], args[3]);
      }
    }
  }
  
  return 0;
}

/* Configure a new network connection. */
int configure(int num, char **args, char *syntax){

  /* Auto configure network connection */
  if(num < 3){

    printf("%s", syntax);
  }

  else if(strcmp(args[1], "auto") == 0 || strcmp(args[1], "-a") == 0){

    if(num < 4){

      printf("%s", syntax);
    }

    else{

      if(conf_configAuto(args[2], args[3]) < 0){

        printf("Error: Unsuccessful auto configuration of network %s\n", args[2]);
      }

      else{

         printf("Successful auto configuration of network %s!\n", args[2]);
      }
    }
  }
      
  /* Auto configure eap network connection */
  else if(strcmp(args[1], "eap") == 0 || strcmp(args[1], "-e") == 0){

    if(num < 5){

      printf("%s", syntax);
    }

    else{

      if(conf_configAutoEAP(args[2], args[3], args[4]) < 0){

        printf("Error: Unsuccessful auto configuration of eap network %s\n", args[2]);
      }

      else{

        printf("Successful auto configuration of eap network %s!\n", args[2]);
      }
    }
  }

  return 0;
}

/* List all available connections */
int list(int num, char **args, char *syntax){

  char buffer[BUFSIZ]; 
  if(num < 2){

    if(listConfigured(buffer, BUFSIZ) < 0){

      printf("Error: Unsuccessful listing of configured networks\n");
    }

    else{

      printf("Configured networks:\n%s\n", buffer);
    }

    bzero(buffer, BUFSIZ);
    if(listAvailable(buffer, BUFSIZ) < 0){

      printf("Error: Unsuccessful listing of available networks\n");
    }

    else{

      printf("Available networks:\n%s\n", buffer);
    }
  }

  /* List configured networks only */
  else {

    if(strcmp(args[1], "configured") == 0 || strcmp(args[1], "-c") == 0){

      if(listConfigured(buffer, BUFSIZ) < 0){

        printf("Error: Unsuccessful listing of configured networks\n");
      }

      else{

        printf("Configured networks:\n%s\n", buffer);
      }
    }

    /* List available networks only */
    if(strcmp(args[1], "available") == 0 || strcmp(args[1], "-a") == 0){

      if(listAvailable(buffer, BUFSIZ) < 0){

        printf("Error: Unsuccessful listing of available networks\n");
      }

      else{

        printf("Available networks:\n%s\n", buffer);
      }
    }
  }

  return 0;
}

int current(int num, char **args, char *syntax){ 

  char *current = malloc(sizeof(char) * 32);
  strncpy(current, currConnection(), sizeof(char) * 32);
  if(current != NULL){

    printf("%s\n", current);
  }

  else{

    printf("Not connected to a network!\n");
  }

  free(current);
}

void surf(){

  if(surf_init() < 0){

    printf("Error: Unsuccessfully connection to wpa_supplicant\n");
  }

  else{

    printf("Success connecting to wpa_supplicant!\n");
  }
}

/* Exit program */
int do_exit(int num, char **args, char *syntax){ 

  return 1;
}

/* Main program */
int main (int argc, char **argv) {
  
  /* Initiate API at start of program */
  surf();

  /* Run command loop */
  command_loop();
}
