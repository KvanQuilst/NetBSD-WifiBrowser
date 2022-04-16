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
CMD_PROC(say_hello);
CMD_PROC(do_exit);
CMD_PROC(configAuto);
CMD_PROC(configAutoEAP);
CMD_PROC(addEntry);
CMD_PROC(editNetwork);
CMD_PROC(enableNetwork);
CMD_PROC(deleteNetwork);
CMD_PROC(cleanNetworks);
CMD_PROC(lsConfigured);
CMD_PROC(lsAvailable); 

/* Place holder */
int say_hello (int num, char **args, char *syntax) {
  printf ("Hello! ... args[0] is %s\n", args[0]);
  return 0;
}

int do_exit (int num, char **args, char *syntax) {
  return 1;
}

/* 
 * Auto configure a new network. Requires an ssid and psk from the user. 
 * Automatically supplies the additional info for wpa_supplicant to the 
 * configuration file. 
*/ 
int configAuto(int num, char **args, char *syntax){

  char ssid[FIELDLEN], psk[FIELDLEN];
  printf("ssid: %s\n", args[1]); //fgets(ssid, FIELDLEN, stdin);
  printf("psk: %s\n", args[2]); //fgets(psk, FIELDLEN, stdin);
  if(conf_configAuto(ssid, psk) < 0){
    
    printf("Error setting auto configuration. \n");
    return 0;
  }

  else{

    printf("Success setting auto configuration. \n");
    return 1;
  }
}

/* 
 * Auto configures a new EAP network. Requires an ssid, username and password from 
 * the user. Automatically supplies the additional data needed for wpa_supplicant to 
 * the configuration file. 
*/ 
int configAutoEAP(int num, char **args, char *syntax){

  char ssid[FIELDLEN], user[FIELDLEN], pwd[FIELDLEN];
  printf("ssid: "); fgets(ssid, FIELDLEN, stdin);
  printf("user: "); fgets(user, FIELDLEN, stdin);
  printf("pwd: "); fgets(pwd, FIELDLEN, stdin);

  if(conf_configAutoEAP(ssid, user, pwd) < 0){

    printf("Error setting auto configuration for EAP network.\n");
    return 0;
  }

  else{

    printf("Success setting auto configuration for EAP network.\n");
    return 1;
  }
}

/* 
 * Adds a new entry to the conguration file. Used to manually configure a network. 
 * Requires an ssid from the user. Additional fields required if any. Used in 
 * conjunction with edit network and enable network to enable the network and add 
 * additional fields.   
*/
int addEntry(int num, char **args, char *syntax){

  char ssid[FIELDLEN]; 
  printf("ssid: "); fgets(ssid, FIELDLEN, stdin);
  
  if(conf_addEntry(ssid) < 0){

    printf("Error adding new entry.\n");
    return 0;
  }

  else{

    printf("Type 'n' to end additional fields for new network.\n");
    while(TRUE){

      char field[FIELDLEN], value[FIELDLEN];
      printf("field: "); fgets(field, FIELDLEN, stdin);
      printf("value: "); fgets(value, FIELDLEN, stdin);

      if(strcmp(field, "n\n") == 0 || strcmp(value, "n\n") == 0){

        break;
      }

      else{

        if(conf_editNetwork(ssid, field, value) < 0){

          printf("Error editing specified network field to configuration file.\n");
        }

        else{

          printf("Field added to configuration file.\n");
        }
      }
    }

    if(conf_enableNetwork(ssid) < 0){

      printf("Error enabling new network in configuration file.\n");
      return 0;
    }

    else{

      printf("Success enabling new network in configuration file.\n");
      return 1;
    }
  }
}

/* 
 * Edits the specified field for a network. Requires an ssid, field, and value from 
 * the user. Changes to the field will be updated in the configuration file. 
*/
int editNetwork(int num, char **args, char *syntax){

  char ssid[FIELDLEN], field[FIELDLEN], value[FIELDLEN];
  printf("ssid: "); fgets(ssid, FIELDLEN, stdin);
  printf("field: "); fgets(field, FIELDLEN, stdin);
  printf("value: "); fgets(value, FIELDLEN, stdin);

  if(conf_editNetwork(ssid, field, value) < 0){

    printf("Field %s for %s has been changed. \n", field, ssid);
    return 0;
  }

  else{

    printf("Error editing specified network in configuration file. \n");    
    return 1;
  }
}

/* 
 * Enables the specified network in the configuration file. Requires an ssid from the user. 
*/ 
int enableNetwork(int num, char **args, char *syntax){

  char ssid[FIELDLEN];
  printf("ssid: "); fgets(ssid, FIELDLEN, stdin);

  if(conf_enableNetwork(ssid) < 0){

    printf("Error enabling network.\n");
    return 0;
  }

  else{

    printf("Network %s has been enabled.\n", ssid);
    return 1;
  }
}

/* 
 * Deletes the specified network from the configuration file. Requires an ssid from the user.
*/ 
int deleteNetwork(int num, char **args, char *syntax){

  char ssid[FIELDLEN];
  printf("ssid: "); fgets(ssid, FIELDLEN, stdin);

  if(conf_deleteNetwork(ssid) < 0){

    printf("Error deleting %s network from configuration file.\n");
    return 0;
  }

  else{

    printf("Success deleting %s network from configuration file.\n");
    return 1;
  }
}

/* 
 * Lists configured networks in the configuration file. 
*/ 
int lsConfigured(int num, char **args, char *syntax){

  char buffer[LINELEN];
  if(listConfigured(buffer, sizeof(buffer)) < 0){

    printf("Error listing configured networks.\n");
    return 0;
  }

  else{

    printf("Configured Networks: %s\n", buffer);
    return 1;
  }
}

/* 
 * Lists available visible networks. 
*/ 
int lsAvailable(int num, char **args, char *syntax){

  char buffer[LINELEN];
  if(listAvailable(buffer, sizeof(buffer)) < 0){

    printf("Error listing available networks.\n");
    return 0;
  }

  else{

    printf("Available Networks: %s\n", buffer);
    return 1;
  }
}

int cleanNetworks(int num, char **args, char *syntax){

  return 1;
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
