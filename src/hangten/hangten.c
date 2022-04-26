/*
 * HangTen / hangten.c
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * * This software is under the terms of the BSD license.  * See README for more details.
 *
 * XLib graphical user interface for Surf API
 */

#include <stdlib.h>
//#include <surf.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* Globals */
/*XWMHints    *xwmh;
XSizeHints  *xsh;
Display     *p_disp;
Window      *Main, Exit;
GC          theGC,
            exitGC;
XEvent      theEvent;
int         Done = 0;
char        default_geometry[80];

int main(int argc, char **argv)
{
  Colormap  default_cmap;
  XColor    color;
  int       bitmask;
  XGCValues gcv;
  XSetWindowAttributes xswa;

  if ((p_disp = XOpenDisplay("HangTen")) == NULL) {
    fprintf(stderr, "HangTen: can't open display!\n");
    exit(1);
  }

  
}*/

main()
{
  // Open a display.
  Display *d = XOpenDisplay(0);

  if ( d )
    {
      // Create the window
      Window w = XCreateWindow(d, DefaultRootWindow(d), 0, 0, 200,
			       100, 0, CopyFromParent, CopyFromParent,
			       CopyFromParent, 0, 0);

      // Show the window
      XMapWindow(d, w);
      XFlush(d);

      // Sleep long enough to see the window.
      sleep(10);
    }
  return 0;
}
