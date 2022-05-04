/*
 * HangTen / button.h
 * Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
 * * This software is under the terms of the BSD license.  * See README for more details.
 *
 * XLib graphical user interface for Surf API
 */
#pragma once

typedef struct XBUTTON_T *XButton;
struct XBUTTON_T {
  Window  win; 
  int x, y, w, h;
  XTextItem ti;
  void (*press)();
};

XButton buttonCreate(Window p, int x, int y,
    int width, int height, char *text, void (*press)());

//void buttonInitColor();

void buttonExpose(XButton b);
void buttonPress(XButton b);
void buttonRelease(XButton b);
