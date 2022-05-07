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
#include "color.h"
#include "global.h"

#define BUF_SIZE 2048

Display *dpy;
Window w;
int scr;

XFontStruct *font;

Window l;
XTextItem *lti;
char buf[BUF_SIZE];
static void scanList();
static void knownList();
static int separate();

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

  void (*funs[4])() = {
    //scanList,
    knownList,
    NULL,
    NULL,
    NULL
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
  font = XLoadQueryFont(dpy, "fixed");
  if (!font) {
    fprintf(stderr, "Hangten: error loading font\n");
    exit(1);
  }
  if (surf_init() < 0) {
    fprintf(stderr, "Hangten: error connecting to wireless device\n");
    exit(1);
  }
  initColor();

  /* Parent Window */
  w = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), 0, 0, 500,
         500, 0, BlackPixel(dpy,scr), c_grey.pixel);

  XSelectInput(dpy, w, ExposureMask | KeyPressMask);
  XMapWindow(dpy, w);

  for (int i = 0; i < 4; i++) {
    buttons[i] = buttonCreate(w, 10+i*110, 10, 100, 25, labels[i], funs[i]);
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

static void listWin(int (*listfun)(char *, size_t))
{
  int num;

  if (!listfun)
    return;

  if ((listfun)(buf, BUF_SIZE) < 0) {
    strcpy(buf, "Could not scan");
  } else if ((num = separate()) < 0) return;

  if (!l) {
    l = XCreateSimpleWindow(dpy, w, 10, 45, 480, 450,
        0, BlackPixel(dpy, scr), WhitePixel(dpy, scr));
    if (!l) return;

    XSelectInput(dpy, l, ExposureMask | KeyPressMask);
    XMapWindow(dpy, l);

    XDrawLine(dpy, l, gc_dgrey,
        0, 0, 479, 0);
    XDrawLine(dpy, l, gc_dgrey,
        0, 0, 0, 449);
    XDrawLine(dpy, l, gc_lgrey,
        479, 0, 479, 449);
    XDrawLine(dpy, l, gc_lgrey,
        0, 449, 479, 449);
  } else {
    XClearWindow(dpy, l);

    XDrawLine(dpy, l, gc_dgrey,
        0, 0, 479, 0);
    XDrawLine(dpy, l, gc_dgrey,
        0, 0, 0, 449);
    XDrawLine(dpy, l, gc_lgrey,
        479, 0, 479, 449);
    XDrawLine(dpy, l, gc_lgrey,
        0, 449, 479, 449);
  }

  if (lti) free(lti);

  lti = malloc(num * sizeof(XTextItem));
  if (!lti) return;

  for (int i = 0; i < num; i++) {
    lti[i].nchars = strnlen(lti[i].chars, BUF_SIZE);
    lti[i].delta = 0;
    lti[i].font = font->fid;

    XDrawText(dpy, l, DefaultGC(dpy, scr),
      5, 15+(font->ascent+5)*i, &lti[i], 1);
  }
}
static void scanList() { listWin(*listAvailable); }
static void knownList() { listWin(*listConfigured); }

static int separate()
{
  int cnt = 1, num = 0;

  for (int i = 0; i < BUF_SIZE; i++)
    if (buf[i] == '\n') {
      buf[i] = 0;
      num++;
    }

  if (lti) free(lti);
  lti = malloc(num * sizeof(XTextItem));
  if (!lti) return -1;

  lti[0].chars = buf; 
  int i = 0;
  while (cnt < num || i < BUF_SIZE) {
    if (buf[i] == 0) {
      lti[cnt].chars = &buf[i+1];
      cnt++; i++;
    }
    i++;
  }
  return num;
}
