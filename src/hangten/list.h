/*
 * HangTen / list.h
 * Copyright (c) 2022 Dylan Eskew
 * * This software is under the terms of the BSD license.  * See README for more details.
 */

typedef struct LISTITEM_T *XListItem;
typedef XListItem *XList;
struct LISTITEM_T {
  Window win;
  int x, y, w, h;
  XTextItem ti;
  (void) (*select)();
};

XList listCreate(Window p, int n, int x, int y, 
    int w, int h, (void) (*select)());
