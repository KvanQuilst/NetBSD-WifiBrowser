/*
 * HangTen / color.h
 * Copyright (c) 2022 Dylan Eskew
 * * This software is under the terms of the BSD license.  * See README for more details.
 */
#pragma once

extern Colormap colormap;
extern XColor c_grey, c_lgrey, c_dgrey;
extern XGCValues gcv_lgrey, gcv_dgrey;
extern GC  gc_lgrey, gc_dgrey;

void initColor();
