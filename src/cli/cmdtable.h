/*	$NetBSD: $	*/

/* 
 * Copyright (c) 1994 Philip A. Nelson. 
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

/*
 *  cmdtable.h - this is where the command table is defined.
 *
 *  The user must complete this command table.  It is started.
 */

/* procedure definitions. */
#ifndef NO_HELP
CMD_PROC (help);
#endif

/* 
 * Define new commands in this file.  
*/ 

/* hdsetup commands. */
CMD_PROC (disconnect);
CMD_PROC (configure);
CMD_PROC (current);
CMD_PROC (do_exit);
CMD_PROC (forget);
CMD_PROC (clear);
CMD_PROC (edit);
CMD_PROC (conn);
CMD_PROC (list);
CMD_PROC (add);

/* The command definitions. This is where the user should add new
   command definitions.

   Field definition:

   { proc_name, "command_name", "Syntax", "Help" }
   
   NOTE:  For an alphabetical list from the help command, list the
   commands in alphabetical order on the name field. */

const struct command cmd_table [] = {		/* Command Table */

#ifndef NO_HELP
{ help, "help", "HELP [<command>].", "Provides help for all listed <command>s.  If there none, prints a list \n of the commands." },
#endif 

#ifndef NO_HELP
{ help, "?", "", "Prints a list of commands." },
#endif

{ conn,        "connect", "connect [SSID]\n", 
               "Connect to [SSID] network in configuration file.\n"
               "--ssid     The ssid of the network in the configuration file"},
{ disconnect,  "disconnect", "disconnect\n", 
               "Disconnect from an already existing network connection."},
{ add,         "add",  "add [SSID]\n", 
               "Add a new network to the configuration file.\n"
               "--ssid        The ssid of the network added to the configuration file"},
{ forget,      "forget", "forget [SSID]\n", 
               "Removes an already existing network in the configuration file.\n"
               "--ssid        The ssid of the network to be removed from the configuration file"},
{ edit,        "edit",  "edit [SSID]\n" 
                        "edit [SSID] [FIELD] [VALUE]\n",
               "Edits an already existing network in the configuration file.\n"
               "--ssid        The ssid of the network in the configuration file\n"
               "--field       The field of the network to be edited in the configuration file\n"
               "--value       The value the field will be changed to of the network in the configuration file"},
{ configure,   "configure", "configure auto [SSID] [PASSKEY]\n" 
                            "configure eap [SSID] [USER] [PASSWORD]\n",
               "Auto configures a network in the configuration file and enables the network connection.\n"
               "--ssid        The ssid of the network to be auto configured in the configuration file\n"
               "--passkey     The passkey of the network [SSID]\n\n"

               "Auto configures an eap network in the configuration file and enabbles the network connection.\n"
               "--ssid        The ssid of the eap network to be auto configured in the configuration file\n"
               "--user        The username of the eap network [SSID]\n"
               "--password    The password of the user [USER] of the eap network [SSID]"},
{ list,        "list",  "list\n", 
               "Lists all available networks and configured networks in the configuration file.\n"
               "-a           List all available networks only\n"
               "-c           List all configured networks in the configuration file"},
{ current,     "current", "current\n", 
               "Returns the SSID of the current network connection"},
{ do_exit,     "exit",  "exit\n", 
                        "Exit API interface and return to terminal"},
{ clear,       "clear",  "clear\n", 
                        "Clear screen and display terminal prompt"},
};

#define CMDLEN  (sizeof (cmd_table) / sizeof (struct command))

/* The prompt! */
#define PROMPT "cli (? for help): "
