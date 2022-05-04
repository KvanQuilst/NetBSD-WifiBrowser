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
#include "color.h"
#include "global.h"

#define TEXT_LEN 32

/*Colormap colormap;
XColor c_grey, c_lgrey, c_dgrey;
XGCValues gcv_lgrey, gcv_dgrey;
GC gc_lgrey, gc_dgrey;*/

XButton buttonCreate(Window p, int x, int y, 
    int width, int height, char *text, void (*press)())
{
  Window win;
  XButton button = malloc(sizeof(struct XBUTTON_T));
  if (!button)
    return NULL;

  win = XCreateSimpleWindow(dpy, p, x, y, width, height,
      1, BlackPixel(dpy, scr), c_grey.pixel);
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
  button->press = press;

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
  XDrawLine(dpy, b->win, gc_lgrey,
      0, 0, b->w-1, 0);
  XDrawLine(dpy, b->win, gc_lgrey,
      0, 0, 0, b->h-1);
  XDrawLine(dpy, b->win, gc_dgrey,
      b->w-1, 0, b->w-1, b->h-1);
  XDrawLine(dpy, b->win, gc_dgrey,
      0, b->h-1, b->w-1, b->h-1);

  XDrawText(dpy, b->win, DefaultGC(dpy, scr),
      (b->w-XTextWidth(font, b->ti.chars, b->ti.nchars))/2,
      (b->h-(font->ascent+font->descent))/2+font->ascent,
      &b->ti, 1);
}

void buttonPress(XButton b)
{
  XDrawLine(dpy, b->win, gc_dgrey,
      0, 0, b->w-1, 0);
  XDrawLine(dpy, b->win, gc_dgrey,
      0, 0, 0, b->h-1);
  XDrawLine(dpy, b->win, gc_lgrey,
      b->w-1, 0, b->w-1, b->h-1);
  XDrawLine(dpy, b->win, gc_lgrey,
      0, b->h-1, b->w-1, b->h-1);

  if (b->press) (*b->press)();
}

void buttonRelease(XButton b)
{
  XDrawLine(dpy, b->win, gc_lgrey,
      0, 0, b->w-1, 0);
  XDrawLine(dpy, b->win, gc_lgrey,
      0, 0, 0, b->h-1);
  XDrawLine(dpy, b->win, gc_dgrey,
      b->w-1, 0, b->w-1, b->h-1);
  XDrawLine(dpy, b->win, gc_dgrey,
      0, b->h-1, b->w-1, b->h-1);
}
