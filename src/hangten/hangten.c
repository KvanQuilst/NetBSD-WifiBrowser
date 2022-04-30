/*
 * HangTen / hangten.c
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * * This software is under the terms of the BSD license.  * See README for more details.
 *
 * XLib graphical user interface for Surf API
 */

#include <stdio.h>
#include <stdlib.h>
#include <surf.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main()
{
  Display   *dpy;
  int       scr;
  Window    w, c;
  XEvent    ev;

  int x, y, width, height, border_width, depth;
  Window root_win;
  Colormap colormap;
  XColor button_color;
  XColor light_grey, dark_grey;
  XGCValues gcv_lightgrey, gcv_darkgrey;
  GC gc_lightgrey, gc_darkgrey;

  XFontStruct *font;
  XTextItem ti[1];

  /* Open connection to XServer */
  dpy = XOpenDisplay(NULL);
  if (!dpy) {
    fprintf(stderr, "Hangten: Could not open display!");
    exit(1);
  }

  scr = DefaultScreen(dpy);

  /* Parent Window */
  w = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), 0, 0, 500,
         500, 0, BlackPixel(dpy,scr), WhitePixel(dpy, scr));

  XSelectInput(dpy, w, ExposureMask | KeyPressMask);
  XMapWindow(dpy, w);


  /* Child Window */
  colormap = DefaultColormap(dpy, scr);
  XParseColor(dpy, colormap, "rgb:cc/cc/cc", &button_color);
  XAllocColor(dpy, colormap, &button_color);

  /* Light Grey */
  XParseColor(dpy, colormap, "rgb:ee/ee/ee", &light_grey);
  XAllocColor(dpy, colormap, &light_grey);
  gcv_lightgrey.foreground = light_grey.pixel;
  gcv_lightgrey.background = button_color.pixel;
  gc_lightgrey = XCreateGC(dpy, RootWindow(dpy, scr), 
      GCForeground | GCBackground, &gcv_lightgrey);


  /* Dark Grey */
  XParseColor(dpy, colormap, "rgb:88/88/88", &dark_grey);
  XAllocColor(dpy, colormap, &dark_grey);
  gcv_darkgrey.foreground = dark_grey.pixel;
  gcv_darkgrey.background = button_color.pixel;
  gc_darkgrey = XCreateGC(dpy, RootWindow(dpy, scr), 
      GCForeground | GCBackground, &gcv_darkgrey);

  c = XCreateSimpleWindow(dpy, w, 199, 199, 100, 25,
      1, BlackPixel(dpy, scr), button_color.pixel);

  XSelectInput(dpy, c, 
      ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask);
  XMapWindow(dpy, c);

  XGetGeometry(dpy, c, &root_win, &x, &y, &width, &height,
      &border_width, &depth);

  while (1) {
    XNextEvent(dpy, &ev);
    if (ev.xany.window == c) {
      if (ev.type == Expose) {
          XDrawLine(dpy, c, gc_lightgrey,
              0, 0, width-1, 0);
          XDrawLine(dpy, c, gc_lightgrey,
              0, 0, 0, height-1);
          XDrawLine(dpy, c, gc_darkgrey,
              width-1, 0, width-1, height-1);
          XDrawLine(dpy, c, gc_darkgrey,
              0, height-1, width-1, height-1);

          font = XLoadQueryFont(dpy, "7x14");
          ti[0].chars = "Scan Networks";
          ti[0].nchars = 13;
          ti[0].delta = 0;
          ti[0].font = font->fid;
          XDrawText(dpy, c, DefaultGC(dpy, scr),
              (width-XTextWidth(font, ti[0].chars, ti[0].nchars))/2,
              (height-(font->ascent+font->descent))/2+font->ascent, 
              ti, 1);
      }

      if (ev.type == ButtonPress) {
        if (ev.xbutton.button == 1) {
          XDrawLine(dpy, c, gc_darkgrey,
              0, 0, width-1, 0);
          XDrawLine(dpy, c, gc_darkgrey,
              0, 0, 0, height-1);
          XDrawLine(dpy, c, gc_lightgrey,
              width-1, 0, width-1, height-1);
          XDrawLine(dpy, c, gc_lightgrey,
              0, height-1, width-1, height-1);
        }
      }

      if (ev.type == ButtonRelease)
        if (ev.xbutton.button == 1) {
          XDrawLine(dpy, c, gc_lightgrey,
              0, 0, width-1, 0);
          XDrawLine(dpy, c, gc_lightgrey,
              0, 0, 0, height-1);
          XDrawLine(dpy, c, gc_darkgrey,
              width-1, 0, width-1, height-1);
          XDrawLine(dpy, c, gc_darkgrey,
              0, height-1, width-1, height-1);
        }
    }
  }

  return 0;
}
