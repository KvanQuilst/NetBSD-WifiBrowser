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
CMD_PROC (do_exit);
CMD_PROC (forget);
CMD_PROC (conn);
CMD_PROC (conf);
CMD_PROC (edit);
CMD_PROC (list);
CMD_PROC (add);

/* Connect to an already existing network */
int conn(int num, char **args, char *syntax){

  if(num < 2)
    printf("Usage: %s\n", syntax);
  else
    if(conf_enableNetwork(args[1]) < 0)
      printf("Error connecting to %s.\n", args[1]);
    else
      printf("Network %s connected!\n", args[1]);

  return 0;
}

/* Disconnect from an already exisiting connection */
int disconnect(int num, char **args, char *syntax){

  if(num < 2)
    printf("Usage: %s\n", syntax);
  else
    if(conf_disableNetwork(args[1]) < 0)
      printf("Error disconnecting from %s.\n", args[1]);
    else
      printf("Network %s disconnected!\n", args[1]);

  return 0;
}

/* Add a new network to the configuration file */
int add(int num, char **args, char *syntax){

  if(num < 2)
      printf("Usage: %s\n", syntax);
    else
      if(conf_addEntry(args[1]) < 0)
        printf("Error adding %s.\n", args[1]);
      else
        printf("Success adding %s!\n", args[1]);

  return 0;
}

/* Forget a network in the configuration file */
int forget(int num, char **args, char *syntax){

   if(num < 2)
    printf("Usage: %s\n", syntax);
  else
    if(conf_deleteNetwork(args[1]) < 0)
      printf("Error deleting network %s.\n", args[1]);
    else
      printf("Network %s deleted!\n", args[1]);

  return 0;
}

/* Edit a network in the configuration file */
int edit(int num, char **args, char *syntax){

   if(num < 2)
    printf("Usage: %s\n", syntax);
  else
    if(num < 4){
      char field[FIELDLEN], value[FIELDLEN]; 
      printf("field: "); fgets(field, FIELDLEN, stdin); 
      printf("value: "); fgets(value, FIELDLEN, stdin); 
      field[strlen(field) - 1] = 0; 
      value[strlen(value) - 1] = 0; 

      if(conf_editNetwork(args[1], field, value) < 0)
        printf("Error editing specified field to network.\n " 
                  "%s, %s, %s\n", args[1], field, value); 
      else
        printf("Success editing specified field to network.\n "
                "%s, %s, %s\n", args[1], field, value);
    }

    else
      if(conf_editNetwork(args[1], args[2], args[3]) < 0)
        printf("Error editing specified field to network.\n" 
                  "%s, %s, %s\n", args[1], args[2], args[3]); 
      else
        printf("Success editing specified field to network.\n"
                "%s, %s, %s\n", args[1], args[2], args[3]);
                
  return 0;
}

/* Configure a new network connection. */
int configure(int num, char **args, char *syntax){

  /* Auto configure network connection */
  if(num < 3)
    printf("Usage: %s\n", syntax);
  else if(strcmp(args[1], "auto") == 0 || strcmp(args[1], "-a") == 0)
    if(num < 4)
      printf("Usage: %s\n", syntax);
    else
      if(conf_configAuto(args[2], args[3]) < 0)
        printf("Error setting auto configuration of network %s.\n", args[2]);
      else
        printf("Success setting auto configuration of network %s!\n", args[2]);
      
  /* Auto configure eap network connection */
  else if(strcmp(args[1], "eap") == 0 || strcmp(args[1], "-e") == 0)
    if(num < 5)
      printf("Usage: %s\n", syntax);
    else
      if(conf_configAutoEAP(args[2], args[3], args[4]) < 0)
        printf("Error setting auto configuration of eap network %s.\n", args[2]);
      else
        printf("Success setting auto configuration of eap network %s!\n", args[2]);
  return 0;
}

/* List all available connections */
int list(int num, char **args, char *syntax){

  char buffer[BUFSIZ];
  if(num < 2){
    printf("Configured networks:\n");
    if(listConfigured(buffer, BUFSIZ) < 0)
        printf("Error listing configured networks.\n");
      else
        printf("%s\n", buffer);

    bzero(buffer, BUFSIZ);
    printf("Available networks:\n");
    if(listAvailable(buffer, BUFSIZ) < 0)
        printf("Error listing available networks.\n");
      else
        printf("%s\n", buffer);
  }

  /* List configured networks only */
  else {
    if(strcmp(args[1], "configured") == 0 || strcmp(args[1], "-c") == 0){
      printf("Configured networks:\n");
      if(listConfigured(buffer, BUFSIZ) < 0)
        printf("Error listing configured networks.\n");
      else
        printf("%s\n", buffer);
    }

    /* List available networks only */
    if(strcmp(args[1], "available") == 0 || strcmp(args[1], "-a") == 0){
      printf("Available networks:\n");
      if(listAvailable(buffer, BUFSIZ) < 0)
        printf("Error listing available networks.\n");
      else
        printf("%s\n", buffer);
    }
  }

  return 0;
}

/* Exit program */
int do_exit(int num, char **args, char *syntax){ 
  return 1;
}

/* Main program */
int main (int argc, char **argv) {
  
  /* Initiate API at start of program */
  if(surf_init() < 0)
    printf("Error connecting to wpa_supplicant.\n");
  else
    printf("Success connecting to wpa_supplicant!\n");


  char *connect = currConnection();
  printf("here\n");
  printf("%s\n", connect);
  free(connect);

  /* Run command loop */
  command_loop();
}
