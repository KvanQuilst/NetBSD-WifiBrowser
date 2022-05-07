/*
 * HangTen / list.c
 * Copyright (c) 2022 Dylan Eskew
 * * This software is under the terms of the BSD license.  * See README for more details.
 */

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

XList listCreate(Window p, int n, int x, int y, 
    int w, int h, (void) (*select)())
{
  if (!p)
    return NULL;

  Window *win = malloc(n * sizeof(Window));
  if (!win)
    return NULL;
  XList list = malloc(n * sizeof(XListItem));
  if (!list)
    return NULL;

  for (int i = 0; i < n; i++) {
    win[i] = XCreateSimpleWinodw(dpy, p, x, y, w, h,
        0, WhitePixel(dpy, scr), WhitePixel(dpy, scr));
    if (!win[i])
      return NULL;

    list[i] = malloc(sizeof(XListItem));
    if (!list[i])
      return NULL;

    list[i]->win = win[i];
    list[i]->x = x;
    list[i]->y = y + (font->ascent+5)*i;
    list[i]->w = w;
    list[i]->h = h;
    list[i]->ti.delta = 0;
    list[i]->ti.font = font->fid;
    list[i]->select = select;
  }

  return list;
}
