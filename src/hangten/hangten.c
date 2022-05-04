/*
 * HangTen / hangten.c
 * Copyright (c) 2022 Dylan Eskew
 * * This software is under the terms of the BSD license.  * See README for more details.
 *
 * XLib graphical user interface for Surf API
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <surf.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "button.h"
#include "global.h"

#define BUF_SIZE 2048

Display *dpy;
Window w;
int scr;

XFontStruct *font;

char buf[BUF_SIZE];

void scanList();

int main()
{
  XButton buttons[4];
  XEvent    ev;

  char *labels[4] = {
    "Scan Networks",
    "Refresh",
    "Connect",
    "Help"
  };

  int x, y, width, height, border_width, depth;
  Window root_win;

  /* Open connection to XServer */
  dpy = XOpenDisplay(NULL);
  if (!dpy) {
    fprintf(stderr, "Hangten: Could not open display!");
    exit(1);
  }

  scr = DefaultScreen(dpy);

  /* Init Globals */
  font = XLoadQueryFont(dpy, "7x14");
  surf_init();

  /* Parent Window */
  w = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), 0, 0, 500,
         500, 0, BlackPixel(dpy,scr), BlackPixel(dpy, scr));

  XSelectInput(dpy, w, ExposureMask | KeyPressMask);
  XMapWindow(dpy, w);

  buttonInitColor();

  for (int i = 0; i < 4; i++) {
    buttons[i] = buttonCreate(w, 10+i*110, 10, 100, 25, labels[i], *scanList);
    if (!buttons[i])
      exit(1);
  }

  while (1) {
    XNextEvent(dpy, &ev);
    for (int i = 0; i < 4; i++) {
      if (ev.xany.window == buttons[i]->win) {
        if (ev.type == Expose)
          buttonExpose(buttons[i]);

        if (ev.type == ButtonPress)
          if (ev.xbutton.button == 1)
          buttonPress(buttons[i]);

        if (ev.type == ButtonRelease)
          if (ev.xbutton.button == 1)
            buttonRelease(buttons[i]);
      }
    }
  }

  return 0;
}

void scanList()
{
  Window s; 
  XTextItem *ti;
  int cnt = 1, num = 0;
  char **list;
  //char *buf = malloc(2048 * sizeof(char));
  if (!buf)
    return;

  if (listAvailable(buf, BUF_SIZE) < 0) {
    strcpy(buf, "Could not scan");
  } else {
    for (int i = 0; i < BUF_SIZE; i++)
      if (buf[i] == '\n') {
        buf[i] = 0;
        num++;
      }
    list = malloc(num * sizeof(char *));
    list[0] = buf; 
    int i = 0;
    while (cnt < num || i < BUF_SIZE) {
      if (buf[i] == 0) {
        list[cnt] = &buf[i+1];
        cnt++; i++;
      }
      i++;
    }
  }

  s = XCreateSimpleWindow(dpy, w, 10, 45, 480, 450,
      0, BlackPixel(dpy, scr), WhitePixel(dpy, scr));
  if (!w)
    return;

  XSelectInput(dpy, s, ExposureMask | KeyPressMask);
  XMapWindow(dpy, s);

  ti = malloc(num * sizeof(XTextItem));

  for (int i = 0; i < num; i++) {
    ti[i].chars = list[i];
    ti[i].nchars = strnlen(list[i], BUF_SIZE);
    ti[i].delta = 0;
    ti[i].font = font->fid;

    XDrawText(dpy, s, DefaultGC(dpy, scr),
      5, 15+(font->ascent+5)*i, &ti[i], 1);
  }
}
