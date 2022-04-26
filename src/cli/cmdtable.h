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
CMD_PROC (say_hello);
CMD_PROC (do_exit);
CMD_PROC (configAuto);
CMD_PROC (configAutoEAP);
CMD_PROC (addEntry);
CMD_PROC (editNetwork);
CMD_PROC (enableNetwork);
CMD_PROC (deleteNetwork);
CMD_PROC (cleanNetworks);
CMD_PROC (lsConfigured);
CMD_PROC (lsAvailable);
CMD_PROC (conf);

/* The command definitions. This is where the user should add new
   command definitions.

   Field definition:

   { proc_name, "command_name", "Syntax", "Help" }

   NOTE:  For an alphabetical list from the help command, list the
   commands in alphabetical order on the name field. */

const struct command cmd_table [] = {		/* Command Table */

{ say_hello, "hello", "HELLO", "Says hello back" },

#ifndef NO_HELP
{ help, "help", "HELP [<command>].", "Provides help for all listed <command>s.  If there none, prints a list \n of the commands." },
#endif

#ifndef NO_HELP
{ help, "?", "", "Prints a list of commands." },
#endif

{ do_exit, "exit", "EXIT", "Just get out of here." }, 
{ configAuto, "conf_auto ssid psk", "CONF_AUTO", "Adds a new network to the configuration file and automatically supplies the info for wpa_supplicant." },
{ configAutoEAP, "conf_auto_eap ssid user pwd", "CONF_AUTO_EAP", "Adds a new eap network to the configuration file and automatically supplies the additional adata needed for wpa_supplicant. "},
{ addEntry, "conf_add ssid", "CONF_ADD_ENTRY", "Adds a new network to the configuration file to manually configure network. "},
{ editNetwork, "conf_edit_network ssid field value", "CONF_EDIT_NETWORK", "Edits the specified network field in the configuration file. "}, 
{ enableNetwork, "conf_enable_network ssid", "CONF_ENABLE_NETWORK", "Enables the specified network to be used in the configuration file. "},
{ deleteNetwork, "conf_delete_network ssid", "CONF_DELETE_NETWORK", "Deletes the specified network from the configuration file. "},
{ cleanNetworks, "clean_networks", "CLEAN_NETWORKS", "Removes all networks from the configuration file. "},
{ lsConfigured, "list_configured", "LIST_CONFIGURED", "Lists all networks in the configuration file. "},
{ lsAvailable, "list_available", "LIST_AVAILABLE", "Lists all available networks. "}, 

{ conf, "conf ", "CONFIGURATION_FILE_METHODS", "Perform operations on the configuration file. Usage: \n"
                     "-a (auto configuration) (ssid, psk)\n"
                     "-ae (auto configuration eap) (ssid, user, pwd)\n"
                     "-ne (add entry to configuration file) (ssid)\n"
                     "-e (edit network in configuration file) (ssid, field, value)\n"
                     "-en (enable network in configuration file) (ssid)\n"
                     "-dn (delete network in configuration file) (ssid)\n"}, 
};

#define CMDLEN  (sizeof (cmd_table) / sizeof (struct command))

/* The prompt! */
#define PROMPT "cli (? for help): "
