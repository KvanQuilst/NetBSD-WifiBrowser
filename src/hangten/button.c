/*
 * HangTen / button.c
 * Copyright (c) 2022 Dylan Eskew
 * * This software is under the terms of the BSD license.  * See README for more details.
 *
 * Generalized button consturctor and control
 */

#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "button.h"
#include "global.h"

#define TEXT_LEN 32

Colormap colormap;
XColor c_button, c_light, c_dark;
XGCValues gcv_light, gcv_dark;
GC gc_light, gc_dark;

XButton buttonCreate(Window p, int x, int y, 
    int width, int height, char *text)
{
  Window win;
  XButton button = malloc(sizeof(struct XBUTTON_T));
  if (!button)
    return NULL;

  win = XCreateSimpleWindow(dpy, p, x, y, width, height,
      1, BlackPixel(dpy, scr), c_button.pixel);
  if (!win)
    return NULL;

  XSelectInput(dpy, win,
      ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask);
  XMapWindow(dpy, win);

  button->win   = win;
  button->x     = x;
  button->y     = y;
  button->w     = width;
  button->h     = height;

  button->ti.chars = malloc(TEXT_LEN * sizeof(char));
  if (!button->ti.chars)
    return NULL;

  strncpy(button->ti.chars, text, TEXT_LEN);
  button->ti.nchars = strnlen(button->ti.chars, TEXT_LEN);
  button->ti.delta  = 0;
  button->ti.font   = font->fid;

  return button;
}

void buttonExpose(XButton b)
{
  XDrawLine(dpy, b->win, gc_light,
      0, 0, b->w-1, 0);
  XDrawLine(dpy, b->win, gc_light,
      0, 0, 0, b->h-1);
  XDrawLine(dpy, b->win, gc_dark,
      b->w-1, 0, b->w-1, b->h-1);
  XDrawLine(dpy, b->win, gc_dark,
      0, b->h-1, b->w-1, b->h-1);

  XDrawText(dpy, b->win, DefaultGC(dpy, scr),
      (b->w-XTextWidth(font, b->ti.chars, b->ti.nchars))/2,
      (b->h-(font->ascent+font->descent))/2+font->ascent,
      &b->ti, 1);
}

void buttonPress(XButton b)
{
  XDrawLine(dpy, b->win, gc_dark,
      0, 0, b->w-1, 0);
  XDrawLine(dpy, b->win, gc_dark,
      0, 0, 0, b->h-1);
  XDrawLine(dpy, b->win, gc_light,
      b->w-1, 0, b->w-1, b->h-1);
  XDrawLine(dpy, b->win, gc_light,
      0, b->h-1, b->w-1, b->h-1);

  XDrawText(dpy, b->win, DefaultGC(dpy, scr),
      (b->w-XTextWidth(font, b->ti.chars, b->ti.nchars))/2,
      (b->h-(font->ascent+font->descent))/2+font->ascent,
      &b->ti, 1);
}

void buttonRelease(XButton b)
{
  XDrawLine(dpy, b->win, gc_light,
      0, 0, b->w-1, 0);
  XDrawLine(dpy, b->win, gc_light,
      0, 0, 0, b->h-1);
  XDrawLine(dpy, b->win, gc_dark,
      b->w-1, 0, b->w-1, b->h-1);
  XDrawLine(dpy, b->win, gc_dark,
      0, b->h-1, b->w-1, b->h-1);

  XDrawText(dpy, b->win, DefaultGC(dpy, scr),
      (b->w-XTextWidth(font, b->ti.chars, b->ti.nchars))/2,
      (b->h-(font->ascent+font->descent))/2+font->ascent,
      &b->ti, 1);
}

void buttonInitColor()
{
  /* Base button color */
  colormap = DefaultColormap(dpy, scr);
  XParseColor(dpy, colormap, "rgb:cc/cc/cc", &c_button);
  XAllocColor(dpy, colormap, &c_button);

  /* Light */
  XParseColor(dpy, colormap, "rgb:ee/ee/ee", &c_light);
  XAllocColor(dpy, colormap, &c_light);
  gcv_light.foreground = c_light.pixel;
  gcv_light.background = c_button.pixel;
  gc_light = XCreateGC(dpy, RootWindow(dpy, scr),
      GCForeground | GCBackground, &gcv_light);

  /* Dark */
  XParseColor(dpy, colormap, "rgb:88/88/88", &c_dark);
  XAllocColor(dpy, colormap, &c_dark);
  gcv_dark.foreground = c_dark.pixel;
  gcv_dark.background = c_button.pixel;
  gc_dark = XCreateGC(dpy, RootWindow(dpy, scr),
      GCForeground | GCBackground, &gcv_dark);
}
