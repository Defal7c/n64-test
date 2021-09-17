#pragma once

#define NTSC 0
#define PAL 1
#define MPAL 2

#define TV_TYPE MPAL //yeah im brazillian, cry about it

#define SCREEN_WD 320
#define SCREEN_HT 240

#define GLIST_LENGTH 2048

extern Gfx glist[];
extern Gfx* glistp;

void RCPInit(Gfx *glistp);
