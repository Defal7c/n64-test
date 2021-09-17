#include <nusys.h>
#include "inc/config.h"

void ClearBackground(u8 r, u8 g, u8 b);
void DrawRect(int x, int y);

static u8 b;
int square_x;
int sqSize = 64;
int sqDir;

void stage00_init(void) {
    square_x = SCREEN_WD/2 - 32;
    b = 255;    
}

void stage00_update(void) {
    if(square_x == SCREEN_WD - sqSize) { sqDir = -1; }
    else if(square_x == 0) { sqDir = 1; }

    square_x += sqDir;
    b -= 5;
}

void stage00_draw(void) {
    glistp = glist;
    RCPInit(glistp);
    ClearBackground(32,32,255);
    DrawRect(square_x, (SCREEN_HT - sqSize)/ 2);

    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);
    nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx),
                   NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
}

void DrawRect(int x, int y) {
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(255,0,0,1) << 16 | GPACK_RGBA5551(255,0,0,1)));  
    gDPFillRectangle(glistp++, x,y,x+64,y+64);
    gDPPipeSync(glistp++);
}

void ClearBackground(u8 r, u8 g, u8 b) {
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
    gDPPipeSync(glistp++);
}
