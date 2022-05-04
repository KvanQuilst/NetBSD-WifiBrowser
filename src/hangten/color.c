/*
 * HangTen / color.c
 * Copyright (c) 2022 Dylan Eskew
 * * This software is under the terms of the BSD license.  * See README for more details.
 */

#include <X11/Xlib.h>

#include "color.h"
#include "global.h"

Colormap colormap;
XColor c_grey, c_lgrey, c_dgrey;
XGCValues gcv_lgrey, gcv_dgrey;
GC  gc_lgrey, gc_dgrey;

void initColor()
{
  /* Base button color */
  colormap = DefaultColormap(dpy, scr);
  XParseColor(dpy, colormap, "rgb:cc/cc/cc", &c_grey);
  XAllocColor(dpy, colormap, &c_grey);

  /* Light */
  XParseColor(dpy, colormap, "rgb:ee/ee/ee", &c_lgrey);
  XAllocColor(dpy, colormap, &c_lgrey);
  gcv_lgrey.foreground = c_lgrey.pixel;
  gcv_lgrey.background = c_grey.pixel;
  gc_lgrey = XCreateGC(dpy, RootWindow(dpy, scr),
      GCForeground | GCBackground, &gcv_lgrey);

  /* Dark */
  XParseColor(dpy, colormap, "rgb:88/88/88", &c_dgrey);
  XAllocColor(dpy, colormap, &c_dgrey);
  gcv_dgrey.foreground = c_dgrey.pixel;
  gcv_dgrey.background = c_grey.pixel;
  gc_dgrey = XCreateGC(dpy, RootWindow(dpy, scr),
      GCForeground | GCBackground, &gcv_dgrey);
}
