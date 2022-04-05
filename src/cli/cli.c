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
#include "command.h"
#include "extern.h"
#include "../api/surf.h" 

/*
 * Include all the functions used with program. 
 *
 */

CMD_PROC(say_hello);
CMD_PROC(do_exit);
CMD_PROC(conf_configAuto);

/* Place holder */

int say_hello (int num, char **args, char *syntax) {
  printf ("Hello! ... args[0] is %s\n", args[0]);
  return 0;
}

int do_exit (int num, char **args, char *syntax) {
  return 1;
}

int conf_configAuto(int num, char **args, char *syntax){

  return 1;
}



/* Main program */
int main (int argc, char **argv) {
  
  /* Initiate API at start of program */
  api_init();

  /* Run command loop */
  command_loop();
}
